let router__devices = document.querySelector('.router--devices');
let router__settings = document.querySelector('.router--settings');
let routers__back_arrow = document.querySelectorAll('.navigation-arrow');

let devices__box = document.querySelector('.devices');
let settings__box = document.querySelector('.settings');
let menu__box = document.querySelector('.router--menu');

router__devices.addEventListener('click', () => {
    menu__box.classList.add('dn');
    devices__box.classList.add('db');
})
router__settings.addEventListener('click', () => {
    menu__box.classList.add('dn');
    settings__box.classList.add('db');
})
routers__back_arrow.forEach((e) => {
    e.addEventListener('click', () => {
        devices__box.classList.remove('db');
        settings__box.classList.remove('db');
        menu__box.classList.remove('dn');
    })
})