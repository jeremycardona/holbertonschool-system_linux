import os

pid = 237046
maps_file = f"/proc/{pid}/maps"
mem_file = f"/proc/{pid}/mem"

with open(maps_file, "r") as maps, open(mem_file, "rb") as mem:
    for line in maps:
        parts = line.split()
        address_range, perms = parts[0], parts[1]
        if 'r' in perms:  # Only read regions with read permissions
            start, end = (int(addr, 16) for addr in address_range.split('-'))
            mem.seek(start)
            data = mem.read(end - start)
            print(f"Data from {hex(start)}-{hex(end)}: {data[:16]}")
