#!/usr/bin/python3
import sys
import os

"""
	main - read and write from proc heap
"""

def main():
    if len(sys.argv) < 4:
        print("Usage: python read_write_heap.py <pid> <search_string> <replace_string>")
        sys.exit(1)
    
    pid = sys.argv[1]
    search_string = sys.argv[2]
    replace_string = sys.argv[3]


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
    main()