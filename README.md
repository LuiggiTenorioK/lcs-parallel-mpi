# Usage

```
mpicc ./main.c -o main
mpirun -np 2 ./main ./input_1000.txt
```

# Accessing CAPRI

```
ssh user@capri.dei.unipd.it
```

# Submitting job

```
sbatch <job_file>
squeue -j <job_id>
```