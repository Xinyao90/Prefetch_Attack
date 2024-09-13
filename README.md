# Prefetch_Attack
## Methods to Check Huge Page Support
### 1. via /proc/cpuinfo
`grep -i "pdpe1gb\|pse" /proc/cpuinfo
`

- This command looks for two key CPU features:

  - pse (Page Size Extension): Indicates support for 4 MB pages.
  - pdpe1gb (Page Directory Pointer Table Entry 1 GB): Indicates support for 1 GB pages.

### 2. via /proc/meminfo
`grep -i huge /proc/meminfo
`
This output will show information such as:

* HugePages_Total: Total number of huge pages configured.
* HugePages_Free: Number of free huge pages.
* Hugepagesize: The size of each huge page (e.g., 2048 kB for 2 MB pages).

### 3. Use dmesg
`dmesg | grep -i huge
`
### 4. via sysfs
`cat /sys/kernel/mm/hugepages/hugepages-*/nr_hugepages
`
### 5. Use the lscpu
`lscpu | grep -i page
`
## Steps to Enable Huge Pages on Linux:

### Enabling Huge Pages on Linux:
```
sudo sh -c 'echo 512 > /proc/sys/vm/nr_hugepages'
sudo mkdir -p /mnt/hugepages
sudo mount -t hugetlbfs nodev /mnt/hugepages
```
### Compiling and Running the Experiment:
```
gcc -o prefetch_hugepage_experiment prefetch_hugepage_experiment.c -O2 -lrt -march=native
sudo ./prefetch_hugepage_experiment
```
## Observing and Measuring Results:

**Performance Counters**: Use perf to measure cache misses, prefetcher events, and TLB misses:
```
sudo perf stat -e L1-dcache-load-misses,L1-dcache-loads,dTLB-load-misses,dTLB-loads ./prefetch_hugepage_experiment
```

`sudo perf stat -e cache-references,cache-misses,LL-prefetches ./prefetch_hugepage_experiment
`
## Steps to Disable Huge Pages on Linux:
### Clear Huge Pages Allocation:

Set the number of huge pages to 0 to release the reserved memory:

This command tells the kernel to release any memory reserved for huge pages.

### Unmount the Huge Pages Filesystem:

Unmount the huge pages filesystem to ensure it is not in use:

If you mounted huge pages on a different mount point, replace /mnt/hugepages with the appropriate path.

### Remove Huge Pages Mount Point (Optional):

If you want to clean up further, you can remove the mount directory created for huge pages:

This step is optional and just for keeping the filesystem tidy.

### Verify Huge Pages Are Disabled:

Check that the huge pages have been released and the system is back to using the default page size:

Ensure that HugePages_Total and HugePages_Free both show 0.

```
sudo sh -c 'echo 0 > /proc/sys/vm/nr_hugepages'
sudo umount /mnt/hugepages
sudo rmdir /mnt/hugepages
grep -i huge /proc/meminfo
```

## Run the Experiment Without Huge Pages:
**Recompile and Run the Experiment:**

To avoid any possible contamination from previous runs, recompile your program:
```
gcc -o prefetch_no_hugepage prefetch_no_hugepage_experiment.c -O2 -lrt -march=native
sudo ./prefetch_no_hugepage
```
## Observing and Measuring Results:

**Performance Counters**: Use perf to measure cache misses, prefetcher events, and TLB misses:
```
sudo perf stat -e L1-dcache-load-misses,L1-dcache-loads,dTLB-load-misses,dTLB-loads ./prefetch_no_hugepage
```

`sudo perf stat -e cache-references,cache-misses,LL-prefetches ./prefetch_no_hugepage
`

