import json
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
import math

colors = ["red", "blue", "green", "orange", "purple", "cyan", "magenta"]

MAX_NODES_FOR_PLOTTING = 500

for idx in range(1, 7):
    input_file = f"../Outputs/output{idx}.json"
    
    try:
        with open(input_file) as f:
            data = json.load(f)
    except FileNotFoundError:
        print(f"Skipping {input_file} (Not found)")
        continue

    all_nodes = set()
    for route in data["routes"].values():
        all_nodes.update(route)
    
    num_unique_nodes = len(all_nodes)
    
    if num_unique_nodes > MAX_NODES_FOR_PLOTTING:
        print(f"Skipping visualization for {input_file}: {num_unique_nodes} unique nodes exceeds the limit of {MAX_NODES_FOR_PLOTTING}.")
        continue

    G = nx.DiGraph()

    for node in all_nodes:
        G.add_node(node)

    edge_colors = []
    edge_list = []

    for i, (veh_id, route) in enumerate(data["routes"].items()):
        color = colors[i % len(colors)]
        for u, v in zip(route, route[1:]):
            G.add_edge(u, v)
            edge_list.append((u, v))
            edge_colors.append(color)

    num_nodes = len(G.nodes)
    plt_figsize = (10, 8)
    k_value = None
    node_s = 500
    font_s = 10
    width_s = 1.5
    arrow_s = 20
    draw_labels = True

    if num_nodes > 50:
        plt_figsize = (24, 24)
        k_value = 2.5 / math.sqrt(num_nodes)
        node_s = 50
        font_s = 6
        width_s = 0.5
        arrow_s = 10
        if num_nodes > 500:
            draw_labels = False

    plt.figure(figsize=plt_figsize)
    pos = nx.spring_layout(G, seed=42, k=k_value, iterations=50)
    nx.draw_networkx_nodes(G, pos, node_color="lightgray", node_size=node_s)
    if draw_labels:
        nx.draw_networkx_labels(G, pos, font_size=font_s)
    nx.draw_networkx_edges(
        G, pos,
        edgelist=edge_list,
        edge_color=edge_colors,
        arrows=True,
        arrowstyle='-|>',
        arrowsize=arrow_s,
        width=width_s,
        connectionstyle="arc3,rad=0.1"
    )

    legend_elements = [Line2D([0], [0], color=colors[i % len(colors)], lw=3, label=f'Vehicle {veh_id}')
                       for i, veh_id in enumerate(data["routes"].keys())]

    plt.legend(handles=legend_elements, loc='upper left', bbox_to_anchor=(1, 1))
    plt.title(f"Route Visualization: {input_file}")
    plt.tight_layout()
    plt.axis('off')
    plt.show()
    plt.clf()