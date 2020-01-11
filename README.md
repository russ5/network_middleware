# network_middleware

Step 0: Middleware is running on every computer.
Step 1: Run application that uses a public mw funcion on node 0.
Step 2: Node 0 application (not mw, which is unused) connects to all other node mws sending config file.
Step 3: All node mws branch/exec to the application then kill mw connection.
Step 4: Topology-specific connections between applications are made.
Step 5: Run application
