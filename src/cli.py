from prompt_toolkit import PromptSession
from prompt_toolkit.completion import Completer, Completion
from prompt_toolkit.shortcuts import CompleteStyle, prompt

commands = {
            "add":
            {
                "user": {
                    "--name": None,
                    "--last_name": None,
                    "--phone": None
                },
                "device": {
                    "--serial": None,
                    "--type": {"alarm": None, "gps": None}
                }
            },
            "assign": {
                "--user_name": None,
                "--device": None
            },
            "remove": {
                "--user": {"user_name": None},
                "--device": {"serial": None}
            },
            "identify" : {
                "--user": {"user_name": None},
                "--device": {"serial": None}
            }
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

def add_command(params):
    add_aval: list = ["user", "device"]
    add_what: str = params[0]

    if add_what not in add_aval:
        print(f"Adding \"{add_what}\" is not avaliable.")
        return

    args = extract_args(params[1:])
    print(args)

def assign_command(params):
    pass

def remove_command(params):
    pass

def identify_command(params):
    pass

callbacks: dict = {
    "add": add_command,
    "assign": assign_command,
    "remove": remove_command,
    "identify": identify_command
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
    
    callbacks[command](parts[1:])

def main():
    session = PromptSession()
    while True:
        try:
            text = session.prompt('> ', completer=MyCustomCompleter(), 
                                  complete_style=CompleteStyle.COLUMN,
                                  complete_while_typing = True)
        except KeyboardInterrupt:
            continue
        except EOFError:
            break
        else:
            parse_inputs(text)

if __name__ == "__main__":
    main()