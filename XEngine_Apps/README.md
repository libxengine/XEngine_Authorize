appclient just only x86 and x64 debug compile on windows

## Authorize_APPWebClient
Web client example (`Authorize_APPWebClient.html`): A browser-based client that demonstrates WebSocket authentication with the XEngine Authorize server. Features include:
- WebSocket long connection to port 5301 with configurable server address
- User login with optional MD5 password encryption
- Heartbeat keep-alive (every 2 seconds)
- Session expiration/timeout notification (AUTH_EXPIRED) with automatic disconnection
- HTTP API support (register, recharge, query time, get announcements, token renewal)
- Dynamic verification code flow
- Auto-reconnect on unexpected disconnect
- Real-time log display

Open the HTML file directly in any modern browser (Chrome/Firefox/Edge/Safari). No build required.
Default credentials: admin / 123123aa