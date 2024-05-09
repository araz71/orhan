#!/usr/bin/python3

import http.server
import socketserver
import urllib.parse
import sys

sys.path.insert(1, "../../cli/")
from db import Database
from cli import perror, psucc

PORT = 8000

class MyHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    db = Database()

    def on_index(self):
        # Serve the default index page
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        self.wfile.write(b'<h1>Welcome to the HTTP server!</h1>')

    def append_column(self, title, msg):
        msg = msg + f"<td width='150px'>{title}</td>"
        return msg

    def on_device(self, url):
        # Handle the '/device' URL
        query = urllib.parse.parse_qs(url.query)
        sn: str = ""
        msg: str = ""
        response: int = 200

        if "sn" in query.keys():
            sn = str(query["sn"][0])
        else:
            response = 500
            msg = "<center><h1>Please enter device serial number!</h1></center>"
        
        if sn:
            # Check in database
            device = self.db.list_devices(pattern=sn)
            if len(device) == 0:
                response = 501
                msg = "<center><h1> Device not found</h1></center>"
            else:
                response = 200
                registers = self.db.list_registers(sn)
                msg = f"<h4>Device {sn} belogs to: {device[0].owner}</h4>"
                msg = msg + f"registers:</br><table><tr>"
                msg = self.append_column("ID", msg)
                msg = self.append_column("Label", msg)
                msg = self.append_column("Type", msg)
                msg = self.append_column("Value", msg)
                msg = self.append_column("Last modification", msg)
                msg = msg + "</tr>"

                for reg in registers:
                    msg = msg + "<tr>"
                    msg = msg + f"<td>{reg.id}</td>"
                    msg = msg + f"<td>{reg.label}</td>"
                    msg = msg + f"<td>{reg.type}</td>"
                    msg = msg + f"<td>{reg.value}</td>"
                    msg = self.append_column(reg.last_modification, msg)
                    msg = msg + "</tr>"

                msg = msg + "</table>"

        self.send_response(response)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        self.wfile.write(bytes(msg, "utf-8"))

    def on_user(self, url):
        pass

    def do_GET(self):
        # Parse the requested URL
        parsed_url = urllib.parse.urlparse(self.path)

        # Handle different URLs
        if parsed_url.path == '/':
            self.on_index()
        elif parsed_url.path == '/device':
            self.on_device(parsed_url)
        elif parsed_url.path == '/user':
            self.on_user(parsed_url)
        else:
            # Return a 404 error for unknown URLs
            self.send_response(404)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            self.wfile.write(b'<h1>404 Not Found</h1>')

with socketserver.TCPServer(("", PORT), MyHTTPRequestHandler) as httpd:
    print(f"Serving at port {PORT}")
    httpd.serve_forever()

httpd.shutdown()