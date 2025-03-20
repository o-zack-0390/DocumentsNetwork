@echo off
REM run.bat - Apacheサーバー起動（フォアグラウンド実行）

echo Starting Apache server in foreground mode...
echo Press Ctrl+C to stop the server.
echo.
echo Server running at 
echo http://localhost:80/
echo.
echo test CGI at
echo http://localhost:80/cgi-bin/test.bat
echo.
echo NetworkGraph CGI at
echo http://localhost:80/cgi-bin/main.cgi
echo.
start http://localhost:80/
start http://localhost:80/cgi-bin/test.bat
start http://localhost:80/cgi-bin/main.cgi
echo.

REM -k start ではなく -X を使用してフォアグラウンドで実行
Apache24\bin\httpd.exe -X -f "%~dp0Apache24\conf\httpd.conf"

REM このコマンドはサーバーが終了するまで戻らない
echo Apache server has been stopped.
pause