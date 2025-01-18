#include <netdb.h>
#include "topology.h"
#include <graphviz/gvc.h>

#pragma region Constructor

Topology::Topology(const std::string &centerPeer) : centerPeer(centerPeer) {
    addPeer(centerPeer);
}

void Topology::addPeer(const std::string &hostname) {
    Peer newPeer;
    newPeer.hostname = hostname;
    peers.push_back(newPeer);

    // trigger next hop calculation on change
    calculateNextHops();
}

void Topology::plot() {
    GVC_t *gvc = gvContext();

    auto g = agopen(nullptr, Agundirected, nullptr);

    std::map<std::string, Agnode_s *> nodes;
    // add all nodes
    for (const auto &peer: peers) {
        nodes.emplace(peer.hostname, agnode(g, (char *) peer.hostname.c_str(), 1));
    }

    // add edges
    std::set<std::string> processed;
    for (const auto &peer: peers) {
        processed.insert(peer.hostname);
        for (const auto &neighbor: peer.neighbors) {
            if (processed.find(neighbor) != processed.end()) continue;
            agedge(g, nodes.find(peer.hostname)->second, nodes.find(neighbor)->second, nullptr, 1);
        }
    }

    gvLayout(gvc, g, "dot");
    auto fp = fopen("plot.png", "w");
    gvRender(gvc, g, "png", fp);

    gvFreeLayout(gvc, g);
    agclose(g);
    fclose(fp);
}

void Topology::calculateNextHops() {
    std::set<std::string> Q;
    // reset
    for (auto &peer: peers) {
        peer.distance = peer.hostname == centerPeer ? 0 : INT32_MAX - 1;
        peer.previous = "";
        Q.insert(peer.hostname);
    }

    while (!Q.empty()) {
        // Peer in Q with minimum distance
        Peer *u;
        int minDistance = INT32_MAX - 1;
        for (const auto &hostname : Q) {
            auto peer = getPeer(hostname);
            if (peer->distance <= minDistance) {
                u = peer;
                minDistance = peer->distance;
            }
        }

        // Remove u from Q
        Q.erase(u->hostname);

        for (const auto &neighborHostname : u->neighbors) {
            auto neighbor = getPeer(neighborHostname);
            if (u->distance + 1 < neighbor->distance) {
                neighbor->distance = u->distance + 1;
                neighbor->previous = u->hostname;
            }
        }
    }

    // update next hops for each peer
    for (auto &peer: peers) {
        // unreachable nodes
        if (peer.distance == INT32_MAX - 1) {
            peer.previous = "";
            peer.nextHop = "";
            continue;
        }
        // if start node or neighbor
        if (peer.previous.empty() || peer.previous == centerPeer) {
            peer.nextHop = peer.hostname;
            continue;
        }

        auto previous = getPeer(peer.previous);
        while (previous != nullptr && previous->previous != centerPeer) {
            previous = getPeer(previous->previous);
        }
        peer.nextHop = previous != nullptr ? previous->hostname : "";
    }
}

#pragma endregion