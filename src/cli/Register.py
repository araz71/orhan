from enum import Enum

class RegisterType(Enum):
    String = "String"
    Integer = "Integer"
    Boolean = "Bool"
    Raw = "Raw"
    Float = "Float"

class Register:
    def __init__(self, sn: str, id: int, type: RegisterType, value: str, label: str,
                 last_modification: str):
        self.sn = sn
        self.id = id
        self.type = type
        self.value = value
        self.label = label
        self.last_modification = last_modification