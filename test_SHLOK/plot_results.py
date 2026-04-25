import pandas as pd
import matplotlib.pyplot as plt

# Load the data
df = pd.read_csv('performance_results.csv')

# Split data
ext4 = df[df['Mode'] == 'Native_Ext4']
flex = df[df['Mode'] == 'Flexclone']

# Plot Throughput
plt.figure(figsize=(10, 6))
plt.plot(ext4['Size_MB'], ext4['Throughput_MBps'], marker='o', label='Native Ext4 (Baseline)', color='blue')
plt.plot(flex['Size_MB'], flex['Throughput_MBps'], marker='s', label='Flexclone (Transactional)', color='red')

plt.title('Filesystem Write Throughput Comparison')
plt.xlabel('File Size (MB)')
plt.ylabel('Throughput (MB/s) - Higher is better')
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend()

plt.savefig('throughput_graph.png')
print("Graph saved as 'throughput_graph.png'")
