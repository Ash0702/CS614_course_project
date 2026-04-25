#!/bin/bash

# 1. Initialize the parent file
touch evic_par_file

# 2. Setup the test file structure
./evict_test_create_file evic_par_file

# 3. Apply extended attributes for three different child/friend relationships
# All mapping to the same log file (evic_logf) with range 0:262143:3
./setxattr_generic -p evic_par_file -c evic_child_1 -f evic_frnd_1 -l evic_logf -r 0:262143:3
./setxattr_generic -p evic_par_file -c evic_child_2 -f evic_frnd_2 -l evic_logf -r 0:262143:3
./setxattr_generic -p evic_par_file -c evic_child_3 -f evic_frnd_3 -l evic_logf -r 0:262143:3

# 4. Perform initial modifications
./evict_test_modify_file evic_par_file
./evict_test_modify_file evic_par_file

# 5. Update the first child file
./update_chilfs evic_child_1

# 6. Modify parent again
./evict_test_modify_file evic_par_file

# 7. Final bulk update of all child files
./update_chilfs evic_child_1
./update_chilfs evic_child_2
./update_chilfs evic_child_3

echo "Eviction test sequence complete."