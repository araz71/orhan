import mysql.connector
import datetime

from User import User
from Device import Device
from Register import Register
from Register import RegisterType

class Database:
    def exec(self, command):
        self.mycursor.execute(command)
        result = self.mycursor.fetchall()
        self.mydb.commit()
        return result

    def __init__(self, host="localhost", username="root", password="12345", db_name="orhandb"):
        # Establish a connection to the MySQL database
        try:
            self.mydb = mysql.connector.connect(
                host=host,
                user=username,
                password=password,
                database=db_name
            )
        except:
            raise Exception("Failed to connect to database.")
        
        if not self.mydb.is_connected():
            raise Exception("Failed to connect to Mysql")

        self.mycursor = self.mydb.cursor()

        # Create the table if it does not exist
        result = self.exec("SHOW TABLES LIKE 'users'")
        if not len(result):
            user_table: str = """CREATE TABLE users(id INT AUTO_INCREMENT PRIMARY KEY,
name Text, last_name Text, phone Text, data_modified TEXT)"""
            result = self.exec(user_table)
            if self.mycursor.rowcount == 0:
                raise Exception("Failed to create user table.")
        
        result = self.exec("SHOW TABLES LIKE 'devices'")
        if not len(result):
            device_table: str = """CREATE TABLE devices(id INT AUTO_INCREMENT PRIMARY KEY,
Serial Text, label TEXT, owner TEXT, date_modified TEXT, date_assigned TEXT, last_connection Text)"""
            result = self.exec(device_table)
            if self.mycursor.rowcount == 0:
                raise Exception("Failed to create device table.")
        
        result = self.exec("SHOW TABLES LIKE 'registers'")
        if self.mycursor.rowcount == 0:
            request  = "CREATE TABLE registers(id INT, sn Text, type Text, value Text, label Text, last_updated Text, History Text)"
            result = self.exec(request)
            if not len(result):
                raise Exception("Failed to create registers table.")

    def add_user(self, name: str, last_name: str, phone: str):
        request: str = f"SELECT * from users where name=\"{name}\""
        result = self.exec(request)
        if result:
            raise Exception(f"User \"{name}\" is defined before.")

        request = f"INSERT INTO users(name, last_name, phone, data_modified) VALUES(\"{name}\", \"{last_name}\", \"{phone}\", \"{datetime.datetime.now()}\")"
        self.exec(request)
        if not self.mycursor.rowcount > 0:
            raise Exception(f"Can not add new user named {name}")

    def add_device(self, serial: str, label: str):
        request: str = f"SELECT * from devices where serial=\"{serial}\""
        result = self.exec(request)
        if result:
            raise Exception(f"Device \"{serial}\" is defined before.")
        
        request = f"INSERT INTO devices(Serial, label, date_modified) VALUES(\"{serial}\", \"{label}\", \"{datetime.datetime.now()}\")"
        self.exec(request)
        if not self.mycursor.rowcount > 0:
            raise Exception(f"Can not add new device with serial {serial}")

    def remove_device(self, serial: str):
        request: str = f"SELECT * from devices where serial=\"{serial}\""
        result = self.exec(request)
        if not result:
            raise Exception(f"Device \"{serial}\" is not found.")

        request = f"DELETE FROM devices where Serial=\"{serial}\""
        result = self.exec(request)
        if self.mycursor.rowcount == 0:
            raise Exception("Removing device \"{serial}\" failed.")

    def remove_user(self, name: str):
        request: str = f"SELECT * from users where name=\"{name}\""
        result = self.exec(request)
        if not result:
            raise Exception(f"User \"{name}\" is not found.")

        request = f"DELETE FROM users where name=\"{name}\""
        result = self.exec(request)
        if self.mycursor.rowcount == 0:
            raise Exception("Removing user \"{name}\" failed.")
    
    def list_users(self, pattern: str = ""):
        users: list = []
        user: User

        request: str = "SELECT * FROM users"
        result = self.exec(request)
        for res in result:
            if pattern in str(res[1]):
                user = User(res[1], res[2], res[3])
                users.append(user)
        
        return users

    def list_devices(self, pattern: str = ""):
        devices: list = []
        device: Device

        request: str = "SELECT * FROM devices"
        result = self.exec(request)
        for res in result:
            if pattern in str(res[1]) or pattern == str(res[1]):
                owner_name: str = "FREE"
                if res[3]:
                    request: str = f"SELECT last_name from users where name=\"{res[3]}\""
                    owner = self.exec(request)
                    if len(owner) and owner[0][0]:
                        owner_name = owner[0][0]

                device = Device(res[1], res[2], owner_name)
                devices.append(device)
        
        return devices

    def assign_device(self, device_sn, user_name):
        request: str = f"SELECT * FROM devices WHERE serial=\"{device_sn}\""
        result = self.exec(request)
        if not len(result):
            raise Exception(f"Device with serial \"{device_sn}\" not found.")
        
        request = f"SELECT * FROM users WHERE name=\"{user_name}\""
        result = self.exec(request)
        if self.mycursor.rowcount == 0:
            raise Exception(f"User with name \"{user_name}\" not found.")
        
        request = f"UPDATE devices SET owner=\"{user_name}\",date_assigned=\"{datetime.datetime.now()}\" WHERE serial=\"{device_sn}\""
        result = self.exec(request)
    
    def add_register(self, sn: str, regID: int, reg_type: RegisterType, label: str):
        request: str = f"SELECT * from registers where id=\"{regID}\" and sn=\"{sn}\""
        result = self.exec(request)
        if len(result):
            raise Exception(f"Register \"{regID}\" on device \"{sn}\" has been added before.")

        request = f"INSERT INTO registers(id,sn,type,value,label) VALUES(\"{regID}\",\"{sn}\", \"{reg_type}\", \"00\", \"{label}\")"
        result = self.exec(request)
        if not self.mycursor.rowcount > 0:
            raise Exception(f"Can not insert register \"{regID}\" on device \"{sn}\"")

    def remove_register(self, sn: str, regID: int):
        request: str = f"SELECT * from registers where id=\"{regID}\" and sn=\"{sn}\""
        result = self.exec(request)
        if len(result):
            raise Exception(f"Register \"{regID}\" not found on device \"{sn}\".")

        request = f"DELETE FROM registers where id=\"{regID}\" and sn=\"{sn}\""
        result = self.exec(request)
        if not len(result):
            raise Exception(f"Can not remove register \"{regID}\" on device \"{sn}\".")

    def get_register(self, sn: str, regID: int):
        request = f"SELECT * FROM registers where sn=\"{sn}\" AND id=\"{regID}\""
        result = self.exec(request);
        if len(result):
            reg = result[0]
            reg: Register = Register(sn, regID, reg[2], reg[3], reg[4], reg[5])
            return reg
        return None

    def set_register(self, sn: str, regID: int, value: str):
        request = f"UPDATE registers SET value=\"{value}\",last_updated=\"{datetime.datetime.now()}\" WHERE sn=\"{sn}\" AND id=\"{regID}\""
        self.exec(request)

    def list_registers(self, sn: str):
        registers: list = []
        request: str = f"SELECT * FROM registers where sn=\"{sn}\""
        result = self.exec(request)
        for reg in result:
            retrieved_reg: Register = Register(reg[1], reg[0], reg[2], reg[3], reg[4], reg[5])
            registers.append(retrieved_reg)

        return registers
