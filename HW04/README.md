
# Job Scheduler Documentation HW4

## Overview

This Job Scheduler allows for the execution of non-interactive background jobs. The scheduler supports running multiple jobs concurrently, with each job's output and errors captured in respective files.

## Features

- **Submit Jobs**: Command `submit` followed by the executable and its arguments.
- **Show Job Status**: Displays running or waiting jobs.
- **Job History**: Lists all jobs with start/end times and statuses.
- **Concurrency Management**: Limits the number of concurrent jobs based on user input.

## File Structure

- **`hw4.c`**: Main source file for the job scheduler.
- **`job<ID>_out.out`**: Output files for job `<ID>`.
- **`job<ID>_err.err`**: Error files for job `<ID>`.


## Compilation

To compile the job scheduler:

```bash
gcc -Wall -o hw4 hw4.c
```

This will create an executable named `hw4`.

## Usage

To run the scheduler, use:

```bash
./hw4 <number_of_parallel_jobs>
```

Replace `<number_of_parallel_jobs>` with the maximum number of jobs to run concurrently.

### Commands

### Submit a Job
  ```
  submit <command>
  ```
  Example: `submit sleep 10`
- **Description:** This feature allows users to enqueue jobs for execution. Each job is specified by a command, which consists of an executable followed by its arguments. The scheduler then queues these jobs according to the available system resources and concurrency limits.

- **Usage:** Users can enter the submit command directly into the scheduler's command prompt. This adds the job to the waiting queue unless slots are immediately available for execution.

### Show Current Jobs
  ```
  showjobs
  ```
- **Description:** This command provides real-time visibility into the status of all jobs that are either currently running or waiting in the queue. This is crucial for monitoring and managing ongoing tasks within the system.

- **Usage:** Executing the showjobs command will display a list that includes the job ID, the command, and the current status (Running or Waiting) of each job.

### Show Job History
  ```
  submithistory
  ```
- **Description:** Keeps a log of all completed jobs, including their start and end times, which helps in auditing and performance monitoring. This historical data is essential for backtracking and understanding the dynamics of job execution over time.

- **Usage:** By running the submithistory command, users can retrieve a detailed list of all jobs that have been processed, including their exact timings and final status.

### Exiting

To exit, use CTRL+C or close the terminal.

## Example Session

```bash
    Enter command> submit ls -R
    Job 1 submitted and is now running.
    Enter command> submit sleep 10
    Job 2 submitted and is now running.
    Enter command> submit ls-rehj
    Job 3 submitted and is now running.
    Enter command> submithistory
    Job History:
    Jobid: 1, Command: 'ls -R', Start: 2024-04-07 00:22:45, End: 2024-04-07 00:22:45, Status: Success
    Jobid: 2, Command: 'sleep 10', Start: 2024-04-07 00:22:59, End: 2024-04-07 00:22:59, Status: Success
    Jobid: 3, Command: 'ls-rehj', Start: 2024-04-07 00:22:59, End: 2024-04-07 00:22:59, Status: Failed
    Enter command> 
```


## Github link

https://github.com/Misritha0810/sysassignments.git