import json
import matplotlib.pyplot as plt
from datetime import datetime

# Data storage
metrics = {
    'fps': {'timestamps': [], 'values': []},
    'render_time_ms': {'timestamps': [], 'values': []},
    'loop_time_ms': {'timestamps': [], 'values': []},
    'idle_time_ms': {'timestamps': [], 'values': []}
}

# Assuming your log file is named 'amiberry_metrics.log'
log_file_path = 'cmake-build-debug/Amiberry.app/Contents/MacOS/amiberry_metrics.log'
try:
    with open(log_file_path, 'r') as f:
        for line in f:
            try:
                data = json.loads(line)
                metric_name = data.get('name')
                if data.get('type') == 'metric' and metric_name in metrics:
                    # Parse timestamp and value
                    timestamp = datetime.fromisoformat(data['timestamp'].replace('Z', ''))
                    value = data['value']
                    metrics[metric_name]['timestamps'].append(timestamp)
                    metrics[metric_name]['values'].append(value)
            except (json.JSONDecodeError, KeyError, TypeError):
                # Ignore lines that aren't valid JSON or are missing expected keys
                continue
except FileNotFoundError:
    print(f"Log file not found at: {log_file_path}")
    exit()

# Create a figure with subplots
fig, axs = plt.subplots(len(metrics), 1, figsize=(12, 15), sharex=True)
fig.suptitle('Amiberry Performance Metrics Over Time', fontsize=16)

# Plot each metric
for i, (name, data) in enumerate(metrics.items()):
    if data['timestamps']:
        axs[i].plot(data['timestamps'], data['values'], marker='o', linestyle='-')
        axs[i].set_title(name.replace('_', ' ').title())
        axs[i].set_ylabel(data.get('unit', ''))
        axs[i].grid(True)

# Common X-axis label
plt.xlabel('Time')

# Improve layout
plt.xticks(rotation=45)
plt.tight_layout(rect=[0, 0.03, 1, 0.96])
plt.show()
