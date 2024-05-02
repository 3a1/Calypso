import requests
import os

offsets_url = "https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/offsets.json"
client_url = "https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/client.dll.json"
offsets_path = "./offsets/offsets.json"
client_path = "./offsets/client.dll.json"

def print_logo():
    print("  ___      _                   ")
    print(" / __|__ _| |_  _ _ __ ___ ___ ")
    print("| (__/ _` | | || | '_ (_-</ _ \\")
    print(" \\___\\__,_|_|\\_, | .__/__/\\___/")
    print("             |__/|_|           \n")

def fetch_raw_data(url):
    try:
        response = requests.get(url)
        if response.status_code == 200:
            return response.content
        else:
            print(f"[-] Failed to fetch data from {url}. Status code: {response.status_code}")
            return None
    except Exception as e:
        print(f"[-] Error fetching offsets from {url}: {e}")
        return None

def save_to_file(data, file_path):
    try:
        os.makedirs(os.path.dirname(file_path), exist_ok=True)
        with open(file_path, 'wb') as file:
            file.write(data)
        print(f"[+] Offsets saved to {file_path} successfully.")
    except Exception as e:
        print(f"[-] Error saving offsets to {file_path}: {e}")

def main():
    os.system('cls')
    print_logo()
    input("[+] Press Enter to download offsets.\n")
    offsets_data = fetch_raw_data(offsets_url)
    client_data = fetch_raw_data(client_url)
    if offsets_data and client_data:
        save_to_file(offsets_data, offsets_path)
        save_to_file(client_data, client_path)
    else:
        print("[-] Error while saving offsets")
    input("\nPress Enter to exit.\n")

if __name__ == "__main__":
    main()
