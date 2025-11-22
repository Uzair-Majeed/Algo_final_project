#!/usr/bin/env python3
"""
Disaster Relief Optimization - Visualization Module
Reads solution from JSON and creates visual representation
"""

import json
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.patches import FancyBboxPatch
import numpy as np

def load_data(input_file='sample_input.json', output_file='sample_output.json'):
    """Load graph and solution data from JSON files"""
    with open(input_file, 'r') as f:
        input_data = json.load(f)
    
    with open(output_file, 'r') as f:
        output_data = json.load(f)
    
    return input_data, output_data

def plot_solution(input_data, output_data, save_path='solution_visualization.png'):
    """Create visualization of the disaster relief solution"""
    
    # Set up the plot
    fig, ax = plt.subplots(figsize=(14, 10))
    fig.suptitle('Disaster Relief Optimization Solution', fontsize=16, fontweight='bold')
    
    # Parse nodes
    nodes = {node['id']: node for node in input_data['nodes']}
    
    # Parse edges
    edges = input_data['edges']
    
    # Parse routes
    routes = output_data['routes']
    
    # Color scheme for priorities
    def get_priority_color(priority):
        if priority == 0:
            return 'black'  # Depot
        elif priority >= 4:
            return 'red'  # High priority
        elif priority >= 3:
            return 'orange'  # Medium priority
        else:
            return 'yellow'  # Low priority
    
    # Draw all edges first (in light gray)
    for edge in edges:
        u, v = edge['u'], edge['v']
        x1, y1 = nodes[u]['x'], nodes[u]['y']
        x2, y2 = nodes[v]['x'], nodes[v]['y']
        
        # Line thickness based on reliability
        reliability = edge.get('reliability', 1.0)
        linewidth = 0.5 + reliability * 1.5
        alpha = 0.3 + reliability * 0.3
        
        ax.plot([x1, x2], [y1, y2], 
                color='lightgray', 
                linewidth=linewidth, 
                alpha=alpha,
                zorder=1)
        
        # Add edge label (cost)
        mid_x, mid_y = (x1 + x2) / 2, (y1 + y2) / 2
        ax.text(mid_x, mid_y, f"{edge['cost']:.0f}", 
                fontsize=7, 
                bbox=dict(boxstyle='round,pad=0.3', facecolor='white', alpha=0.7),
                ha='center',
                zorder=2)
    
    # Draw vehicle routes with different colors
    vehicle_colors = ['blue', 'green', 'purple', 'cyan', 'magenta']
    
    for vehicle_id, route in routes.items():
        vehicle_idx = int(vehicle_id) - 1
        color = vehicle_colors[vehicle_idx % len(vehicle_colors)]
        
        # Draw route path
        for i in range(len(route) - 1):
            u, v = route[i], route[i + 1]
            x1, y1 = nodes[u]['x'], nodes[u]['y']
            x2, y2 = nodes[v]['x'], nodes[v]['y']
            
            # Draw arrow
            ax.annotate('', 
                       xy=(x2, y2), 
                       xytext=(x1, y1),
                       arrowprops=dict(
                           arrowstyle='->', 
                           color=color, 
                           lw=2.5,
                           alpha=0.8
                       ),
                       zorder=3)
            
            # Add step number
            mid_x = x1 + 0.3 * (x2 - x1)
            mid_y = y1 + 0.3 * (y2 - y1)
            ax.text(mid_x, mid_y, str(i+1),
                   fontsize=8,
                   color=color,
                   fontweight='bold',
                   bbox=dict(boxstyle='circle,pad=0.1', 
                            facecolor='white', 
                            edgecolor=color),
                   zorder=4)
    
    # Draw nodes
    for node_id, node in nodes.items():
        x, y = node['x'], node['y']
        priority = node['priority']
        demand = node['demand']
        
        # Node appearance based on priority
        if priority == 0:
            # Depot - black square
            marker = 's'
            size = 300
            color = 'black'
            edgecolor = 'white'
        else:
            marker = 'o'
            size = 200 + priority * 50
            color = get_priority_color(priority)
            edgecolor = 'darkred' if priority >= 4 else 'black'
        
        ax.scatter(x, y, 
                  s=size, 
                  c=color, 
                  marker=marker,
                  edgecolors=edgecolor,
                  linewidths=2,
                  zorder=5,
                  alpha=0.9)
        
        # Node label
        if priority == 0:
            label = f"Depot\n{node_id}"
            fontweight = 'bold'
            fontsize = 10
        else:
            label = f"{node_id}\nP:{priority}\nD:{demand}"
            fontweight = 'normal'
            fontsize = 9
        
        ax.text(x, y - 8, label,
               fontsize=fontsize,
               fontweight=fontweight,
               ha='center',
               va='top',
               bbox=dict(boxstyle='round,pad=0.5', 
                        facecolor='white', 
                        alpha=0.8,
                        edgecolor='gray'),
               zorder=6)
    
    # Add legend
    legend_elements = [
        plt.Line2D([0], [0], color='black', marker='s', markersize=10, 
                   linestyle='', label='Depot'),
        plt.Line2D([0], [0], color='red', marker='o', markersize=10, 
                   linestyle='', label='High Priority (≥4)'),
        plt.Line2D([0], [0], color='orange', marker='o', markersize=10, 
                   linestyle='', label='Medium Priority (3)'),
        plt.Line2D([0], [0], color='yellow', marker='o', markersize=10, 
                   linestyle='', label='Low Priority (<3)', markeredgecolor='black'),
    ]
    
    # Add vehicle routes to legend
    for vehicle_id in routes.keys():
        vehicle_idx = int(vehicle_id) - 1
        color = vehicle_colors[vehicle_idx % len(vehicle_colors)]
        legend_elements.append(
            plt.Line2D([0], [0], color=color, linewidth=2.5, 
                      label=f'Vehicle {vehicle_id} Route')
        )
    
    ax.legend(handles=legend_elements, 
             loc='upper right', 
             fontsize=10,
             framealpha=0.9)
    
    # Add metrics box
    metrics = output_data.get('metrics', {})
    metrics_text = "Performance Metrics:\n"
    metrics_text += f"Total Time: {metrics.get('computation_time', 0)*1000:.2f} ms\n"
    metrics_text += f"Dijkstra: {metrics.get('dijkstra_time', 0)*1000:.2f} ms\n"
    metrics_text += f"Route Construction: {metrics.get('route_construction_time', 0)*1000:.2f} ms\n"
    metrics_text += f"Optimization: {metrics.get('optimization_time', 0)*1000:.2f} ms"
    
    ax.text(0.02, 0.98, metrics_text,
           transform=ax.transAxes,
           fontsize=9,
           verticalalignment='top',
           bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))
    
    # Formatting
    ax.set_xlabel('X Coordinate (km)', fontsize=11)
    ax.set_ylabel('Y Coordinate (km)', fontsize=11)
    ax.grid(True, alpha=0.3, linestyle='--')
    ax.set_aspect('equal')
    
    # Add title with vehicle info
    num_vehicles = len(routes)
    total_locations = sum(len(set(route) - {0}) for route in routes.values())
    ax.set_title(f'{num_vehicles} Vehicles | {total_locations} Locations Served', 
                fontsize=12, pad=10)
    
    plt.tight_layout()
    
    # Save the figure
    plt.savefig(save_path, dpi=300, bbox_inches='tight')
    print(f"✅ Visualization saved to: {save_path}")
    
    # Show the plot
    plt.show()

def create_animated_visualization(input_data, output_data, save_path='solution_animated.gif'):
    """Create an animated visualization showing route progression"""
    from matplotlib.animation import FuncAnimation, PillowWriter
    
    print("Creating animated visualization...")
    
    nodes = {node['id']: node for node in input_data['nodes']}
    routes = output_data['routes']
    
    # Prepare all route steps
    all_steps = []
    for vehicle_id, route in routes.items():
        for i in range(len(route)):
            all_steps.append((vehicle_id, route[:i+1]))
    
    fig, ax = plt.subplots(figsize=(12, 10))
    
    vehicle_colors = ['blue', 'green', 'purple', 'cyan', 'magenta']
    
    def animate(frame):
        ax.clear()
        ax.set_title(f'Disaster Relief Routes - Step {frame+1}/{len(all_steps)}', 
                    fontsize=14, fontweight='bold')
        
        # Draw nodes
        for node_id, node in nodes.items():
            x, y = node['x'], node['y']
            if node['priority'] == 0:
                ax.scatter(x, y, s=300, c='black', marker='s', 
                          edgecolors='white', linewidths=2, zorder=5)
                ax.text(x, y-5, f"Depot", fontsize=10, ha='center', 
                       bbox=dict(boxstyle='round', facecolor='white', alpha=0.8))
            else:
                color = 'red' if node['priority'] >= 4 else 'orange' if node['priority'] >= 3 else 'yellow'
                ax.scatter(x, y, s=200 + node['priority']*50, c=color, 
                          edgecolors='black', linewidths=2, zorder=5, alpha=0.9)
                ax.text(x, y-5, f"{node_id}\nP:{node['priority']}", 
                       fontsize=9, ha='center',
                       bbox=dict(boxstyle='round', facecolor='white', alpha=0.8))
        
        # Draw routes up to current frame
        for step_idx in range(frame + 1):
            vehicle_id, route = all_steps[step_idx]
            vehicle_idx = int(vehicle_id) - 1
            color = vehicle_colors[vehicle_idx % len(vehicle_colors)]
            
            for i in range(len(route) - 1):
                u, v = route[i], route[i + 1]
                x1, y1 = nodes[u]['x'], nodes[u]['y']
                x2, y2 = nodes[v]['x'], nodes[v]['y']
                
                alpha = 0.4 if step_idx < frame else 1.0
                ax.annotate('', xy=(x2, y2), xytext=(x1, y1),
                           arrowprops=dict(arrowstyle='->', color=color, 
                                         lw=2.5, alpha=alpha),
                           zorder=3)
        
        ax.set_xlabel('X Coordinate (km)', fontsize=11)
        ax.set_ylabel('Y Coordinate (km)', fontsize=11)
        ax.grid(True, alpha=0.3)
        ax.set_aspect('equal')
    
    anim = FuncAnimation(fig, animate, frames=len(all_steps), 
                        interval=500, repeat=True)
    
    # Save animation
    writer = PillowWriter(fps=2)
    anim.save(save_path, writer=writer)
    print(f"✅ Animated visualization saved to: {save_path}")
    
    plt.close()

def main():
    """Main function to generate visualizations"""
    print("=" * 70)
    print("DISASTER RELIEF VISUALIZATION")
    print("=" * 70)
    
    try:
        # Load data
        print("\n1. Loading data...")
        input_data, output_data = load_data()
        print("   ✅ Data loaded successfully")
        
        # Create static visualization
        print("\n2. Creating static visualization...")
        plot_solution(input_data, output_data, 'solution_visualization.png')
        
        # Create animated visualization (optional)
        print("\n3. Creating animated visualization (optional)...")
        try:
            create_animated_visualization(input_data, output_data, 'solution_animated.gif')
        except ImportError:
            print("   ⚠️  Pillow not installed. Skipping animation.")
            print("   Install with: pip install pillow")
        except Exception as e:
            print(f"   ⚠️  Animation creation failed: {e}")
        
        print("\n" + "=" * 70)
        print("VISUALIZATION COMPLETE ✅")
        print("=" * 70)
        print("\nGenerated files:")
        print("  - solution_visualization.png (static image)")
        print("  - solution_animated.gif (animated route progression)")
        
    except FileNotFoundError as e:
        print(f"\n❌ Error: {e}")
        print("Make sure to run './disaster_relief sample' first to generate JSON files")
    except Exception as e:
        print(f"\n❌ Error: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    main()

