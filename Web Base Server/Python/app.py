from flask import Flask, send_from_directory, request

app = Flask(__name__, static_folder='public')

@app.route('/')
def serve_html():
    return send_from_directory(app.static_folder, 'index.html')

@app.route('/api/scan', methods=['GET'])
def scan_networks():
    return 'Scanning networks...'

@app.route('/api/sniff', methods=['GET'])
def sniff_traffic():
    return 'Sniffing traffic...'

@app.route('/api/logs', methods=['POST'])
def download_logs():
    return 'Logs downloaded!'

@app.route('/api/settings', methods=['POST'])
def update_settings():
    return 'Settings updated!'

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
