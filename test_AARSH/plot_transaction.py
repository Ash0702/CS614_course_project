import pandas as pd
import matplotlib.pyplot as plt

print("Loading comparison data...")
df = pd.read_csv('microbench_compare_results.csv')

# Average the iterations
avg_df = df.groupby(['Mode', 'Size_MB']).mean().reset_index()

# Convert microseconds to milliseconds
for col in ['Begin_us', 'Write_us', 'End_us', 'Fsync_us']:
    avg_df[col.replace('_us', '_ms')] = avg_df[col] / 1000.0

ext4 = avg_df[avg_df['Mode'] == 'Native_Ext4']
flex = avg_df[avg_df['Mode'] == 'Flexclone']

sizes = ext4['Size_MB'].astype(str)

# Setup subplots (1 row, 2 columns) sharing the Y-axis
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6), sharey=True)
fig.suptitle('Filesystem Write Phase Breakdown: Native Ext4 vs Flexclone', fontsize=14)

# Colors
c_begin = '#ff9999'
c_write = '#66b3ff'
c_end = '#99ff99'
c_fsync = '#ffcc99'

# --- Plot 1: Native Ext4 ---
ax1.bar(sizes, ext4['Write_ms'], label='Writes (VFS + RAM)', color=c_write)
ax1.bar(sizes, ext4['Fsync_ms'], bottom=ext4['Write_ms'], label='Fsync (SSD Flush)', color=c_fsync)
ax1.set_title('Native Ext4 (Baseline)')
ax1.set_xlabel('File Size (MB)')
ax1.set_ylabel('Time Taken (Milliseconds)')
ax1.grid(axis='y', linestyle='--', alpha=0.7)

# --- Plot 2: Flexclone ---
ax2.bar(sizes, flex['Begin_ms'], label='BEGIN_TXN', color=c_begin)
ax2.bar(sizes, flex['Write_ms'], bottom=flex['Begin_ms'], label='Writes (VFS + RAM)', color=c_write)
ax2.bar(sizes, flex['End_ms'], bottom=flex['Begin_ms']+flex['Write_ms'], label='END_TXN (Commit)', color=c_end)
ax2.bar(sizes, flex['Fsync_ms'], bottom=flex['Begin_ms']+flex['Write_ms']+flex['End_ms'], label='Fsync (SSD Flush)', color=c_fsync)
ax2.set_title('Flexclone (Transactional)')
ax2.set_xlabel('File Size (MB)')
ax2.grid(axis='y', linestyle='--', alpha=0.7)

# Add a single legend for the whole figure
handles, labels = ax2.get_legend_handles_labels()
fig.legend(handles, labels, loc='center right', bbox_to_anchor=(0.98, 0.5))

# Adjust layout to make room for legend
plt.tight_layout()
plt.subplots_adjust(right=0.85)

plt.savefig('microbench_compare_graph.png')
print("\nGraph saved as 'microbench_compare_graph.png'")
