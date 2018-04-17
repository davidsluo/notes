import argparse
import math
import random
from typing import TextIO, Tuple

import networkx
from matplotlib import pyplot


class Node:
    def __init__(self, label, predecessor: 'Node' = None, distance=math.inf):
        self.label = label
        self.predecessor = predecessor
        self.distance = distance

    def __str__(self):
        predecessor = self.predecessor.label if self.predecessor is not None else 'N'
        return f'({self.label}, {predecessor}, {self.distance})'

    def __repr__(self):
        predecessor = self.predecessor.label if self.predecessor is not None else 'None'
        return f'Node(label={self.label}, predecessor={predecessor}, distance={self.distance})'

    def __lt__(self, other: 'Node'):
        return self.distance < other.distance

    def __gt__(self, other: 'Node'):
        return self.distance > other.distance

    def __hash__(self):
        return hash(self.label)


def dijkstra(g: networkx.Graph, source: Node):
    q = set(g.nodes)
    source.distance = 0

    while q:
        u = min(q, key=lambda x: x.distance)
        q.remove(u)

        for v in g.neighbors(u):
            try:
                alt = u.distance + g[u][v]['weight']
            except:
                alt = u.distance
            if alt < v.distance:
                v.distance = alt
                v.predecessor = u

    return [(node, node.predecessor) for node in g.nodes.keys() if node.predecessor is not None]


def get_node_labeled(label, graph: networkx.Graph):
    for node in graph.nodes:
        if node.label == label:
            return node


def graph_from_file():
    parser = argparse.ArgumentParser()
    parser.add_argument('input_file', type=argparse.FileType(mode='r'))
    args = parser.parse_args()

    graph = networkx.Graph()

    node_count = None
    start_node = None
    end_node = None
    weight_table = {}

    file: TextIO = args.input_file
    with file as f:
        # read node info
        for _ in range(3):
            key, value = f.readline().split('=', maxsplit=1)
            if key == 'n':
                node_count = int(value)
            elif key == 's':
                start_node = int(value)
            elif key == 't':
                end_node = int(value)

        graph.add_nodes_from([Node(i) for i in range(1, node_count + 1)])

        f.readline()

        for node1 in range(1, node_count + 1):
            for node2, weight in enumerate(f.readline().split(', '), start=1):
                if node1 == node2:
                    continue
                u = next(filter(lambda x: x.label == node1, graph.nodes.keys()))
                v = next(filter(lambda x: x.label == node2, graph.nodes.keys()))
                graph.add_edge(u, v, weight=int(weight))

    return graph


def randomized_graph(num_nodes, edge_density, weight_range: Tuple[int, int]):
    graph = networkx.Graph()
    graph.add_nodes_from((Node(i) for i in range(num_nodes)))
    nodes = list(graph.nodes)

    num_edges = round((num_nodes + 1) * num_nodes / 2 * edge_density)
    i = 0
    while i < num_edges:
        u, v = random.choices(nodes, k=2)
        if not graph.has_edge(u, v):
            graph.add_edge(u, v, weight=random.randint(*weight_range))
            i += 1
    return graph


def random_weight_change(graph: networkx.Graph):
    edges = list(graph.edges)
    u, v = random.choice(edges)
    graph[u][v]['weight'] = random.randint(1, 100)
    return u, v


def draw_frame(graph: networkx.Graph, layout, start):
    path = dijkstra(graph, start)
    graph.graph['path'] = path
    edge = random_weight_change(graph)

    networkx.draw(graph, pos=layout, with_labels=True,
                  node_color='red', font_color='black', font_weight='bold', font_family='Iosevka')
    networkx.draw_networkx_nodes(graph, layout, nodelist=[start], node_color='orange')
    networkx.draw_networkx_edges(graph, layout, edgelist=path, edge_color='red')
    labels = {key: value['weight'] for key, value in graph.edges.items()}
    networkx.draw_networkx_edge_labels(graph, layout, edge_labels=labels, font_family='Iosevka', font_weight='bold')
    networkx.draw_networkx_edge_labels(graph, layout, edge_labels={edge: graph[edge[0]][edge[1]]['weight']},
                                       font_color='red', font_weight='bold')

    # pyplot.text(-0.9, 1, f'MST Weight: {tree_weight}')
    # pyplot.text(-0.9, 0.9, f'Path Weight {path_weight}')


if __name__ == '__main__':
    # graph: networkx.Graph = graph_from_file()
    graph: networkx.Graph = randomized_graph(5, 1, (1, 100))
    start = random.choice(list(graph.nodes))
    layout = networkx.shell_layout(graph)

    for i in range(5):
        figure = pyplot.figure(dpi=300, figsize=(5, 5))
        draw_frame(graph, layout, start)
        random_weight_change(graph)
        figure.savefig(f'{i}.png')
