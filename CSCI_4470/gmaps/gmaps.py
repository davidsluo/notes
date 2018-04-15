import argparse
from typing import TextIO

import networkx
from matplotlib import pyplot

if __name__ == '__main__':
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

        graph.add_nodes_from(range(1, node_count + 1))

        f.readline()

        for node1 in range(1, node_count + 1):
            for node2, weight in enumerate(f.readline().split(', '), start=1):
                if node1 == node2:
                    continue
                graph.add_edge(node1, node2, weight=int(weight))

    pyplot.figure(dpi=300)
    networkx.draw_shell(graph, with_labels=True,
                        node_color='blue', font_color='white', font_weight='bold', font_family='Iosevka',
                        edge_cmap=pyplot.get_cmap('RdYlGn'))
    networkx.draw_networkx_edge_labels(graph, networkx.shell_layout(graph), font_family='Iosevka')
    pyplot.show()
