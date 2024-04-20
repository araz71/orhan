#!/usr/bin/python3

import sys

from prompt_toolkit import PromptSession
from prompt_toolkit.completion import Completer, Completion
from prompt_toolkit.shortcuts import CompleteStyle, prompt
from db import Database
from prettytable import PrettyTable


db = Database()

def missed_argument(arg_name: str):
    print(f"Missed argument: \"{arg_name}\" should be enter.\n")
    return False

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
            "exit": None
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
            last_dict: dict = commands
            for key in keys:
                if key in last_dict.keys() and last_dict[key] != None:
                    last_dict = last_dict[key]
            
            for key in last_dict.keys():
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

def perror(err: str):
    print(f"Err: {err}")

def psucc(msg: str):
    print(f"Successfull: {msg}")

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

def identify_command(params):
    pass

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

def list_register(registers):
    table = PrettyTable(["Index", "ID", "Value"])
    index: int = 1
    for reg in registers:
        table.add_row([index, reg[0], reg[2]])
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

    elems = list_callbacks[list_what][0](pattern)
    table = list_callbacks[list_what][1](elems)
    print(table)

def register_command(params):
    function: str = params[0]
    callbacks: dict = {
        "add": db.add_register,
        "remove": db.remove_register,
        "get": db.get_register,
        "set": db.set_register
    }

    if function not in callbacks.keys():
        raise Exception(f"Register function \"{function}\" is invalid.")
    
    args = extract_args(params[1:])
    if "serial" not in args.keys() and "regID" not in args.keys():
        raise Exception("serial number of device and register number should be enter.")

    callbacks[function](args["serial"], args["regID"])

callbacks: dict = {
    "add": add_command,
    "assign": assign_command,
    "remove": remove_command,
    "identify": identify_command,
    "list": list_command,
    "exit": exit_command,
    "register": register_command
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
        print(f"Err: {e}")

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
    print("Orhan CLI v0.1. Press tab or enter a key to show completer.\n")
    if len(sys.argv) > 1:
        args: str = sys.argv[1]
        for i in range(2, len(sys.argv)):
            args = args + " " + sys.argv[i]
        parse_inputs(args)
    else:
        main()