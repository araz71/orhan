#!/usr/bin/python3

import sys
import os

from prompt_toolkit import PromptSession
from prompt_toolkit.completion import Completer, Completion
from prompt_toolkit.shortcuts import CompleteStyle, prompt
from db import Database
from prettytable import PrettyTable
from Register import RegisterType

db = Database()

def missed_argument(arg_name: str):
    print(f"Missed argument: \"{arg_name}\" should be enter.\n")
    return False

def dict_users():
    users: dict = {}
    user_list = db.list_users()
    for user in user_list:
        users[user.name + "-" + user.label] = None
    return users

def dict_devices():
    devices: dict = {}
    device_list = db.list_devices()
    for dev in device_list:
        devices[dev.serial + "-" + dev.label] = None
    return devices

def dict_registers(sn: str):
    registers: dict = {}
    register_list = db.list_registers(sn)
    for reg in register_list:
        registers[reg.id + "-" + reg.label] = None

commands = {
            "add":
            {
                "user": {
                    "--name": None,
                    "--label": None,
                    "--phone": None
                },
                "device": {
                    "--serial": None,
                    "--label": None,
                }
            },
            "list":
            {
                "users": None,
                "devices": None,
                "registers": {
                    "--serial": None
                }
            },
            "assign": {
                "--name": None,
                "--serial": None
            },
            "remove": {
                "user": {"--name": None},
                "device": {"--serial": None}
            },
            "register": {
                "add": {
                    "--serial": None,
                    "--regID": None,
                    "--label": None,
                    "--type": {
                        member_name.name: None for member_name in RegisterType
                    }
                },
                "remove": {
                    "--serial": None,
                    "--regID": None,
                },
                "set": {
                    "--serial": None,
                    "--regID": None,
                    "--value": None
                },
                "get": {
                    "--serial": None,
                    "--regID": None
                }
            },
            "exit": None,
            "clear": None
}

keywords = commands.keys()

class MyCustomCompleter(Completer):
    def get_completions(self, document, complete_event):
        inputs = document.text
        keys = inputs.split(" ")
        word_before_cursor = document.get_word_before_cursor()
        completer_result: list = []
        final_result: list = []

        if len(inputs) <= 1:
            completer_result = keywords
        else:
            current_dict: dict = commands
            last_input: str = ""

            last_input = keys[-1]
            if len(last_input) == 0:
                last_input = keys[-2]

            if last_input == "--serial":
                current_dict = dict_devices()
            elif last_input == "--name":
                current_dict = dict_users()
            else:
                for i in range(0, len(keys)):
                    key = keys[i]
                    if key in current_dict.keys() and current_dict[key] != None:
                        current_dict = current_dict[key]
        
            for key in current_dict.keys():
                completer_result.append(key)

        for r in completer_result:
            if r not in keys:
                if word_before_cursor:
                    if word_before_cursor in r:
                        final_result.append(r)
                else:
                    final_result.append(r)

        for value in final_result:
            yield Completion(value, start_position=-len(word_before_cursor))

def extract_args(args: str) -> dict:
    result: dict = {}
    if len(args)%2 != 0:
        print("Wrong number of arguments.")
        return {}

    for i in range(0, len(args)):
        if args[i].startswith("--"):
            result[args[i].strip("-")] = args[i + 1]

    return result

def prepare_args(args: dict):
    for key, value in args.items():
        if "-" in value:
            args[key] = value.split("-")[0]

def perror(err: str):
    print(f"\033[31mErr:\033[0m {err}")

def psucc(msg: str):
    print(f"\033[32Successfull:\033[0m {msg}\n")

def add_device(args):
    forced_args = commands["add"]["device"]

    for forced_arg in forced_args.keys():
        if forced_arg.strip("-") not in args.keys():
            return missed_argument(forced_arg)

    db.add_device(args["serial"], args["label"])

def add_user(args):
    forced_args = commands["add"]["user"]

    for forced_arg in forced_args.keys():
        if forced_arg.strip("-") not in args.keys():
            return missed_argument(forced_arg)

    db.add_user(args["name"], args["label"], args["phone"])

def add_command(params):
    add_what: str = params[0]
    add_callback: dict = {
        "user": add_user,
        "device": add_device
    }

    if add_what not in add_callback.keys():
        perror(f"Adding \"{add_what}\" is not avaliable.")
        return

    args = extract_args(params[1:])
    add_callback[add_what](args)
    print(f"Successfull: {add_what} added successfully.")

def assign_command(params):
    args = extract_args(params)
    if "serial" not in args.keys() or "name" not in args.keys():
        raise Exception("User name or device serial number should be enter.")
    
    prepare_args(args)
    db.assign_device(args["serial"], args["name"])

def remove_command(params):
    remove_what: str = params[0]
    remove_callbacks: dict = {
        "user": db.remove_user,
        "device": db.remove_device
    }
    if remove_what not in remove_callbacks.keys():
        perror(f"Removing \"{remove_what}\" is not defined.")

    args = extract_args(params[1:])
    prepare_args(args)

    what: str = ""
    if remove_what == "user":
        if "name" not in args.keys():
            raise Exception("Please enter user name to remove.")
        what = args["name"]
    else:
        if "serial" not in args.keys():
            raise Exception("Please enter device serial number to remove.")
        what = args["serial"]

    remove_callbacks[remove_what](what)
    psucc(f"{remove_what} \"{what}\" removed.")

def exit_command(params):
    exit(0)

def list_devices(devices):
    table = PrettyTable(["Index", "Serial", "Label", "Owner"])
    index: int = 1
    for device in devices:
        table.add_row([index, device.serial, device.label, device.owner])

    return table

def list_users(users):
    table = PrettyTable(["Index", "user-name", "Label", "Phone"])
    index: int = 1
    for user in users:
        table.add_row([index, user.name, user.label, user.phone])
        index = index + 1

    return table

def list_register(params):
    args = extract_args(params)
    prepare_args(args)
    if "serial" not in args.keys():
        raise Exception("To list registers device serial should enter.")

    registers = db.list_registers(args["serial"])
    table = PrettyTable(["ID", "Label", "Type", "Value", "Last modification"])
    index: int = 1
    for reg in registers:
        table.add_row([reg.id, reg.label, reg.type, reg.value, reg.last_modification])
        index = index + 1

    return table

def list_command(params):
    list_callbacks: dict = {
        "users": [db.list_users, list_users],
        "devices": [db.list_devices, list_devices],
        "registers": [db.list_registers, list_register]
    }

    list_what: str = params[0]
    if list_what not in list_callbacks:
        raise Exception(f"Listing \"{list_what}\" is unavaliable")

    pattern: str = ""
    if len(params) >=2 :
        pattern = params[1]

    if list_what == "registers":
        table = list_register(params[1:])
    else:
        elems = list_callbacks[list_what][0](pattern)
        table = list_callbacks[list_what][1](elems)

    print(f"List of {list_what}")
    table.align = "l"
    print(table)
    print()

def register_add_command(args):
    if "type" not in args:
        raise Exception("Please select type of register.")
    
    label: str = "Not assigned"
    if "label" in args:
        label = args["label"]

    db.add_register(args["serial"], args["regID"], args["type"], label)

def register_remove_command(args):
    db.remove_register(args["serial"], args["regID"])

def register_get_command(args):
    pass

def register_set_command(args):
    pass

def register_command(params):
    function: str = params[0]
    callbacks: dict = {
        "add": register_add_command,
        "remove": register_remove_command,
        "get": register_get_command,
        "set": register_set_command
    }

    if function not in callbacks.keys():
        raise Exception(f"Register function \"{function}\" is invalid.")
    
    args = extract_args(params[1:])
    prepare_args(args)
    if "serial" not in args.keys() and "regID" not in args.keys():
        raise Exception("serial number of device and register number should be enter.")

    regID: int = args["regID"]
    callbacks[function](args)
    psucc(f"{function} done on register \"{regID}\".")

callbacks: dict = {
    "add": add_command,
    "assign": assign_command,
    "remove": remove_command,
    "list": list_command,
    "exit": exit_command,
    "register": register_command,
    "clear": lambda x: os.system("clear")
}

def parse_inputs(inputs: str):
    parts: str = inputs.split(" ")
    
    if len(parts) < 1:
        print("Please enter command...")
        return

    command = parts[0]
    if command not in callbacks.keys():
        print(f"Command ({command}) not found!")
        return
    
    try:
        callbacks[command](parts[1:])
    except Exception as e:
        perror(e)

def main():
    session = PromptSession()
    while True:
        try:
            text = session.prompt('orhan-cli > ', completer=MyCustomCompleter(), 
                                  complete_style=CompleteStyle.COLUMN,
                                  complete_while_typing = True)
        except KeyboardInterrupt:
            continue
        except EOFError:
            break
        else:
            parse_inputs(text)

if __name__ == "__main__":
    print("Orhan CLI v0.1. Press tab or enter a key to show completer.")
    if len(sys.argv) > 1:
        args: str = sys.argv[1]
        for i in range(2, len(sys.argv)):
            args = args + " " + sys.argv[i]
        parse_inputs(args)
    else:
        print()
        main()