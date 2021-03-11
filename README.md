# 9-axis-sensor
9軸センサを用いた姿勢推定を頑張っているところです．  
<各fileの説明>  
ROLL_PITCH_YAW.fzz: 配線図
9-axis data.ino: 9軸センサから得られる生データをそのままmicroSDに保存するためのarduinoプログラム  
processing_test_file.ino: 9軸センサの生データを，madgwickフィルタを使って姿勢角roll,pitch, yawに変換して，serialに出力するためのarduinoプログラム  
sketch__.pde: processing_test_file.inoと同時に用いる．arduinoのserialから角度を持ってきて，画像をprocessingプログラム  
