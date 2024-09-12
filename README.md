# Prefetch_Attack

## Steps to Disable Huge Pages:
### Clear Huge Pages Allocation:

Set the number of huge pages to 0 to release the reserved memory:

`sudo sh -c 'echo 0 > /proc/sys/vm/nr_hugepages'`
