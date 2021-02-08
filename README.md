# 9-axis-sensor
9軸センサを用いた試製推定を頑張っているところです．\n
<各fileの説明>\n
fritzing: 配線図\n
9-axis data.ino: 9軸センサから得られる生データをそのままmicroSDに保存するためのarduinoプログラム\n
processing_test_file.ino: 9軸センサの生データを，madgwickフィルタを使って姿勢角roll,pitch, yawに変換して，serialに出力するためのarduinoプログラム\n
sketch__.pde: processing_test_file.inoと同時に用いる．arduinoのserialから角度を持ってきて，画像processingプログラム\n
