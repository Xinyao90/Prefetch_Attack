# Prefetch_Attack

## Steps to Disable Huge Pages:
### Clear Huge Pages Allocation:

Set the number of huge pages to 0 to release the reserved memory:

`sudo sh -c 'echo 0 > /proc/sys/vm/nr_hugepages'`
This command tells the kernel to release any memory reserved for huge pages.

### Unmount the Huge Pages Filesystem:

Unmount the huge pages filesystem to ensure it is not in use:

`sudo umount /mnt/hugepages`
If you mounted huge pages on a different mount point, replace /mnt/hugepages with the appropriate path.

### Remove Huge Pages Mount Point (Optional):

If you want to clean up further, you can remove the mount directory created for huge pages:

`sudo rmdir /mnt/hugepages`
This step is optional and just for keeping the filesystem tidy.

### Verify Huge Pages Are Disabled:

Check that the huge pages have been released and the system is back to using the default page size:
`grep -i huge /proc/meminfo
`
Ensure that HugePages_Total and HugePages_Free both show 0.

## Run the Experiment Without Huge Pages:
**Recompile and Run the Experiment:**

To avoid any possible contamination from previous runs, recompile your program:
`gcc -o prefetch_no_hugepage prefetch_no_hugepage_experiment.c -O2 -lrt -march=native
`
**Run the Experiment Without Huge Pages:**
`./prefetch_no_hugepage
`


