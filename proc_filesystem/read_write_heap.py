#!/usr/bin/python3

"""
This script finds a string in the heap of a running process and replaces it.

Usage: read_write_heap.py <pid> <search_string> <replace_string>
where pid is the pid of the running process
and strings are ASCII.
The script looks only in the heap of the process.
Output: prints relevant information or error messages.
On usage error, prints an error message on stdout and exits with status code 1.
"""

import sys
import os


def read_write_heap(pid, search_string, replace_string):
    """
    Reads the heap of a process and replaces a string.

    Args:
        pid (str): The process ID.
        search_string (str): The string to search for.
        replace_string (str): The string to replace with.

    Raises:
        PermissionError: If the script does not have permission to read/write the process memory.
        FileNotFoundError: If the process does not exist.
        Exception: For any other exceptions.
    """
    if len(replace_string) > len(search_string):
        print("Error: replace_string must not be longer than search_string")
        sys.exit(1)

    try:
        with open(f"/proc/{pid}/maps", "r") as maps_file:
            heap_info = None
            for line in maps_file:
                if "[heap]" in line:
                    heap_info = line
                    break

        if heap_info is None:
            print("Error: Could not find heap segment")
            sys.exit(1)

        heap_start, heap_end = [int(x, 16) for x in heap_info.split(" ")[0].split("-")]

        with open(f"/proc/{pid}/mem", "rb+") as mem_file:
            mem_file.seek(heap_start)
            heap = mem_file.read(heap_end - heap_start)

            index = heap.find(search_string.encode())
            if index == -1:
                print("Error: search_string not found in heap")
                sys.exit(1)

            mem_file.seek(heap_start + index)
            mem_file.write(replace_string.encode() + b'\x00' * (len(search_string) - len(replace_string)))

            print(f"Replaced '{search_string}' with '{replace_string}' in process {pid} heap")

    except PermissionError:
        print("Error: Permission denied. Try running as root.")
        sys.exit(1)
    except FileNotFoundError:
        print("Error: Process not found")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: read_write_heap.py <pid> <search_string> <replace_string>")
        sys.exit(1)

    pid = sys.argv[1]
    search_string = sys.argv[2]
    replace_string = sys.argv[3]

    read_write_heap(pid, search_string, replace_string)