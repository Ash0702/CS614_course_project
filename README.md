
# Transactional Versioning for FlexClone System

This project implements a parent-centric, versioned storage architecture for the Flexclone filesystem framework. By consolidating all physical data into a single append-only parent file, child files function as lightweight metadata pointers to specific versions. 

The system guarantees crash consistency and data integrity through transactional ioctls, CRC-based verification, and atomic commits. A dedicated log file manages logical-to-physical version mapping, while a background garbage collector reclaims unused blocks, ensuring high performance for file cloning and point-in-time recovery.

## Authors
- Aarsh Jain
- Pranay Surepally
- Saksham Verma
- Shlok Jain
- Yatharth Sharma

## Prerequisites

- Linux Kernel (Specific version required)
- `gcc` and `make` for compiling utilities
- A dedicated partition (e.g., `/dev/sda4`) for mounting the filesystem

## Setup Instructions

- Set up the appropriate Linux kernel version by following the instructions given [here](https://docs.google.com/document/d/1rbG8HCmiUK0iL0ruxs2LDO6vu6xZX2RRaEo-ikDUv2Q/edit?tab=t.0).
- Clone this repository and compile the provided utilities.
- Keep the mount point directory name to be /flexclone_mtpt as our test files assume the mount point to be 
  /flexclone_mtpt
- Copy the "binaries" to /flexclone_mtpt, as it contains all the neccessary binaries for testing
- Use /utils_AARSH/setxattr.c to set parent-child-log-friend relation
- Use /utils_AARSH/update_chilfs.c to update a child via IOCTL call

## Usage Guide

Follow these steps to initialize the module and perform versioned cloning operations.

### 1. Initialize the Module and Filesystem
Insert the custom kernel module and mount your device using the `ext4-module` type:

```bash
# Insert the module
./insert_ext4_module.sh

# Mount the device
sudo mount -t ext4-module /dev/sda4 /flexclone_mtpt
```

### 2. Prepare the Parent File
Create the primary data source (parent file) and write initial content:

```bash
echo "initial data" > par
```

### 3. Establish Versioning Relations
Link the parent, child, friend, and log files. The `-r` flag defines the range in the format `0:<number_of_blocks>:3`.

```bash
./setxattr_generic -p par -c child -f frnd -l logf -r 0:0:3
```

### 4. Perform a Transactional Write
Write new data to a specific offset in the parent file. 

```bash
./binaries/write_offset par <offset> "new version data"
```

**Note on Consistency:** 
- Reading the **parent** file will show the updated content immediately.
- Reading the **child** file will still show the *previous* version, as it is pinned to a specific point-in-time.

### 5. Update the Child Clone
To synchronize the child file to the most recent state of the parent, run the update utility:

```bash
./binaries/update_chilf child
```

The child file will now reflect the latest data committed to the parent.

## Architecture Highlights

- **Append-Only Model:** Parent files are never overwritten; modifications are appended to prevent data loss.
- **Log-Based Mapping:** A dedicated log file tracks version IDs to physical disk locations.
- **Atomic Commits:** Uses custom ioctl bounds to ensure intermediate states are never exposed to child views.
- **Garbage Collection:** Automatically frees blocks no longer referenced by any active Flexclone version.

