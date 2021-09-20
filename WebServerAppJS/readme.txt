настройки системы
1 sudo apt-get update

2. sudo nano /boot/config.txt
      [добавить в конец (секция All) disable_splash=1] - убирает радужную панель вначале

3. sudo nano /etc/lightdm/lightdm.conf
   найти строку #[Seat:*] и раскоментировать ее - это уберет курсор мыши совсем 

   {убрать строку содержащую xserver-command=X и вместо нее написать xserver-command=X -nocursor уберет курсор мыши
   sudo apt-get install unclutter} -- скрывает если не используется

4. gui hide bakcadge, pahel and delete desctop image

5. sudo apt-get install xscreensaver  - отключение ухода в сон
  визуальные настройки: 
  пуск -> preference -> screensaver -> disable screensaver -> reboot rasp

-----------------------------------------------------------------
settings app 
1. curl -sL https://deb.nodesource.com/setup_16.x | sudo bash -
	sudo apt install nodejs
[optionals]        sudo npm install -gnpm@7.20.0

2. install electron:
sudo npm install --save-dev electron
	copy reposit to work-dir...

3. sudo npm install pm2@latest -g
	cd work-dir 
	pm2 start npm -- test
	pm2 startup (copy link and exec)
 	pm2 save	



----------------------------------------
проект 
       cd D:\dreamcode\Iot\WebServerAppJS
       npm install
(модификации package.json секция scrip заменить: 
               "test": "concurrently --kill-others \"electron .\" \"vue-cli-service serve\"")
       npm run serve (start морда)
       npm run test (start electron) npm -- test
pm2 start npm --name "testapp" -- start
---------------------------------------------------------------
bags
1 имя не отчищается в окне смены имени
2 убрать раскрашивание кнопок зеленым в самом html (приводит к багам с визуалкой)
3 кнопки вк выкл внешний вид (куда делся, вроде бы делали)


