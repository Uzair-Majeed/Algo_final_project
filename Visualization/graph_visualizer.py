import json
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
import os

os.makedirs("images", exist_ok=True)

colors = ["red", "blue", "green", "orange", "purple"]

for idx in range(1, 7):
    input_file = f"../Outputs/output{idx}.json"
    output_image = f"images/output{idx}.png"

    with open(input_file) as f:
        data = json.load(f)

    G = nx.Graph()

    all_nodes = set()
    for route in data["routes"].values():
        all_nodes.update(route)
    for node in all_nodes:
        G.add_node(node)

    edge_colors = []
    for i, (veh_id, route) in enumerate(data["routes"].items()):
        color = colors[i % len(colors)]
        for u, v in zip(route, route[1:]):
            G.add_edge(u, v, color=color)
            edge_colors.append(color)

    pos = nx.spring_layout(G, seed=42, k=max(0.1, len(G.nodes())/20), iterations=100)
    nx.draw(G, pos, with_labels=True, edge_color=edge_colors, node_color="lightgray", node_size=500)

    legend_elements = [Line2D([0], [0], color=colors[i % len(colors)], lw=3, label=f'Vehicle {veh_id}')
                       for i, veh_id in enumerate(data["routes"].keys())]
    plt.legend(handles=legend_elements, loc='best')

    plt.show()
    plt.savefig(output_image)
    plt.clf()  
    print(f"Saved {output_image}")
