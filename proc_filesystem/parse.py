pid = 237046
maps_file = f"/proc/{pid}/maps"

with open(maps_file, "r") as maps:
    for line in maps:
        parts = line.split()
        address_range, perms, pathname = parts[0], parts[1], parts[-1] if len(parts) > 5 else ''
        print(f"Address Range: {address_range}, Permissions: {perms}, Pathname: {pathname}")
