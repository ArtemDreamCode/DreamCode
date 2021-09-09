let devices_box = document.querySelector('.devices__box');
let devices__items = devices_box.querySelectorAll('.devices__item');
devices__items.forEach(e => {
    e.addEventListener('click', () => {
        e.classList.toggle('device--active');
    })
})