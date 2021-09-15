<template>
    <div class = "home-page">
        <div class="home-page__title title">
            <span class = "copyright">HDC (с) 2021</span>
        </div>
        <div class="controls-button">
            <a :href="message__update != 0 || devs != 0 ? '/settings' : '/?error=no_devs'" class="constrols__item router--settings">
                <div v-if="message__update > 0"   class="message-count">
                    <span class = "message-count__number">{{message__update}}</span>
                </div>
                <svg class = "controls__icon" width="50" height="50" viewBox="0 0 120 120" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M96.1578 64.726L96.0877 65.2866L96.5317 65.6359L107.08 73.9349C107.081 73.9352 107.081 73.9355 107.082 73.9359C107.623 74.364 107.801 75.1658 107.427 75.8613L97.4343 93.1495L97.4281 93.1603L97.4221 93.1711C97.0884 93.783 96.2941 94.0573 95.6069 93.8156L83.1759 88.7733L82.6447 88.5579L82.1903 88.9074C79.6481 90.8629 77.024 92.4646 73.9686 93.7256L73.4389 93.9442L73.3597 94.5117L71.5117 107.747C71.3877 108.457 70.7593 109 70 109H50C49.2408 109 48.6124 108.457 48.4884 107.747L46.6404 94.5117L46.5599 93.9352L46.0189 93.7205C42.9795 92.5144 40.3604 90.8694 37.8098 88.9074L37.3554 88.5579L36.8242 88.7733L24.3932 93.8156C23.706 94.0573 22.9117 93.783 22.5779 93.1711L22.572 93.1603L22.5658 93.1495L12.5658 75.8495L12.5659 75.8495L12.561 75.8413C12.1755 75.1889 12.3471 74.3877 12.9188 73.9356C12.9191 73.9354 12.9194 73.9351 12.9197 73.9349L23.4684 65.6359L23.9124 65.2866L23.8423 64.726C23.6446 63.1445 23.5001 61.5907 23.5001 60C23.5001 58.408 23.6448 56.803 23.8432 55.1168L23.9093 54.5547L23.4614 54.2086L12.9197 46.0651C12.9185 46.0642 12.9173 46.0632 12.9161 46.0622C12.3465 45.6098 12.1761 44.81 12.561 44.1587L12.561 44.1587L12.5658 44.1504L22.5658 26.8504L22.572 26.8397L22.5779 26.8288C22.9316 26.1806 23.7381 25.9165 24.3714 26.1755L24.3714 26.1756L24.3774 26.178L36.8274 31.178L37.3497 31.3877L37.8 31.05C40.357 29.1323 42.9785 27.486 46.0189 26.2795L46.5599 26.0648L46.6404 25.4883L48.4884 12.2531C48.6124 11.5429 49.2408 11 50 11H70C70.7593 11 71.3877 11.5429 71.5117 12.2531L73.3597 25.4883L73.4401 26.0648L73.9812 26.2795C77.0216 27.486 79.6431 29.1323 82.2 31.05L82.6504 31.3877L83.1727 31.178L95.6227 26.178L95.6287 26.1755C96.2619 25.9165 97.0685 26.1806 97.4221 26.8288L97.4281 26.8397L97.4343 26.8504L107.427 44.1386C107.8 44.8331 107.623 45.6336 107.084 46.0622C107.083 46.0632 107.082 46.0641 107.08 46.0651L96.5387 54.2086L96.0908 54.5547L96.1569 55.1168C96.3553 56.803 96.5 58.408 96.5 60C96.5 61.5907 96.3554 63.1445 96.1578 64.726ZM46.9186 73.0815C50.388 76.5509 55.0935 78.5 60 78.5C64.9066 78.5 69.6121 76.5509 73.0815 73.0815C76.5509 69.612 78.5 64.9065 78.5 60C78.5 55.0935 76.5509 50.3879 73.0815 46.9185C69.6121 43.4491 64.9066 41.5 60 41.5C55.0935 41.5 50.388 43.4491 46.9186 46.9185C43.4492 50.3879 41.5 55.0935 41.5 60C41.5 64.9065 43.4492 69.612 46.9186 73.0815Z" stroke="#75A4FF" stroke-width="3"/>
                </svg>
            </a>
            <a :href="devs != 0 ? '/devices' : '/?error=no_old_devs'" class="constrols__item router--devices">
                <svg class = "controls__icon" width="50" height="50" viewBox="0 0 118 100" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M59 16.1917L88.1667 42.4417V88H76.5V53H41.5V88H29.8333V42.4417L59 16.1917ZM59 0.5L0.666656 53H18.1667V99.6667H53.1667V64.6667H64.8333V99.6667H99.8333V53H117.333L59 0.5Z" stroke="#75A4FF" stroke-width="3"/>
                </svg>
            </a>
            <div class="constrols__item">
                <svg class = "controls__icon" width="50" height="50" viewBox="0 0 120 120" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M63.5 36.5V43.5H56.5V36.5H63.5ZM18.5 60C18.5 82.8784 37.1216 101.5 60 101.5C82.8784 101.5 101.5 82.8784 101.5 60C101.5 37.1216 82.8784 18.5 60 18.5C37.1216 18.5 18.5 37.1216 18.5 60ZM41.4399 15.1918C47.3242 12.7545 53.6309 11.5 60 11.5C66.3691 11.5 72.6759 12.7545 78.5602 15.1918C84.4444 17.6292 89.791 21.2017 94.2947 25.7053C98.7983 30.209 102.371 35.5556 104.808 41.4398C107.246 47.3241 108.5 53.6309 108.5 60C108.5 72.863 103.39 85.1992 94.2947 94.2947C85.1992 103.39 72.863 108.5 60 108.5C53.6309 108.5 47.3242 107.246 41.4399 104.808C35.5556 102.371 30.209 98.7983 25.7053 94.2947C16.6098 85.1992 11.5 72.863 11.5 60C11.5 47.137 16.6098 34.8008 25.7053 25.7053C30.209 21.2017 35.5556 17.6292 41.4399 15.1918ZM63.5 56.5V83.5H56.5V56.5H63.5Z" stroke="#75A4FF" stroke-width="3"/>
                </svg>
            </div>
            <div class="constrols__item">
                <svg class = "controls__icon" width="50" height="50" viewBox="0 0 80 20" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M61 10C61 7.61305 61.9482 5.32387 63.636 3.63604C65.3239 1.94821 67.6131 1 70 1C72.3869 1 74.6761 1.94821 76.364 3.63604C78.0518 5.32387 79 7.61305 79 10C79 12.3869 78.0518 14.6761 76.364 16.364C74.6761 18.0518 72.3869 19 70 19C67.6131 19 65.3239 18.0518 63.636 16.364C61.9482 14.6761 61 12.3869 61 10ZM31 10C31 7.61305 31.9482 5.32387 33.636 3.63604C35.3239 1.94821 37.6131 1 40 1C42.3869 1 44.6761 1.94821 46.364 3.63604C48.0518 5.32387 49 7.61305 49 10C49 12.3869 48.0518 14.6761 46.364 16.364C44.6761 18.0518 42.3869 19 40 19C37.6131 19 35.3239 18.0518 33.636 16.364C31.9482 14.6761 31 12.3869 31 10ZM1 10C1 7.61305 1.94821 5.32387 3.63604 3.63604C5.32387 1.94821 7.61305 1 10 1C12.3869 1 14.6761 1.94821 16.364 3.63604C18.0518 5.32387 19 7.61305 19 10C19 12.3869 18.0518 14.6761 16.364 16.364C14.6761 18.0518 12.3869 19 10 19C7.61305 19 5.32387 18.0518 3.63604 16.364C1.94821 14.6761 1 12.3869 1 10Z"  stroke="#75A4FF" stroke-width="3"/>
                </svg>
            </div>
        </div>
        <div v-show = "message.show" class="messages-container">
            <transition name="fade">
                <Message :text = "message.text" :title = "message.title" />
            </transition>
        </div>
    </div>
</template>

<script>
import { io } from "socket.io-client";
import Message from "@/components/Message.vue";


export default {
  name: 'Menu',
  props: {
  },
  methods: {
  },
  data() {
      return {
          message__update: 0,
          devs: 0,
          message : {
              title : 'Важное сообщение',
              text : '',
              show: false,
          }
      }
  },
    components: {
      Message
  },
  mounted() {
	const socket = io("http://localhost:3000/");

		if (localStorage.devices_old) {
			this.devs = JSON.parse(localStorage.getItem("devices_old"));
		}
		if (localStorage.CountNewDev) {
			var devices = JSON.parse(localStorage.getItem("CountNewDev"));
            this.message__update = devices
		}
		socket.on("devices_old", devices => { 
			this.devs = devices;
			localStorage.setItem("devices_old", JSON.stringify(devices))
        })
        socket.on("CountNewDev", devices => {
            this.message__update = devices
			localStorage.setItem("CountNewDev", devices)
        })

    if(this.$route.query.error == "no_devs") {
        this.message.text = "Не найдено никаких устройств"
        this.message.show = true;
    } else if (this.$route.query.error == "no_old_devs") {
        this.message.text = "Не найдено подключенных устройств"
        this.message.show = true;
    }
  }
}
</script>

<style scoped>
.home-page {
    display: flex;
    flex-direction: column;
    justify-content: space-between;
}
.copyright {
    font-size: 1rem;
    font-weight: 400;
    color:gray;
}
.constrols__item:hover .controls__icon path {
    stroke: #6495ed;
}
.home-page__title{
    margin-bottom: 25vh;
}
.controls-button {
    display: flex;
    flex-direction: row;
    width: 100%;
    justify-content: center;
    align-items: center;
    flex-wrap: wrap;
}
.constrols__item {
    padding: 1rem;
    border:2px solid var(--blue);
    cursor: pointer;
    border-radius: 15px;
    margin-right: 1.1rem;
    position: relative;
    margin-bottom: 1.1rem;
}
.controls__inner {
    min-height: 50vh;
    display: flex;
    align-items: center;
    justify-content: center;
}
.message-count {
    border-radius: 50%;
    background: var(--red);
    display: flex;
    justify-content: center;
    align-items: center;
    width: 40px;
    height: 40px;
    padding: 0.3rem 0.7rem;
    position: absolute;
    top: -13%;
    right: -13%;
    overflow: hidden;
}
.message-count__number {
    color:var(--white);
    font-size: 1.5rem;
}
@media (max-width:730px) {
    .constrols__item {
        margin-bottom: 1rem;
    }
    .controls-button {
        justify-content: center;
    }
}
@media (max-width: 480px) {
    .vue-notification-group {
        margin-bottom: 5% !important;
        width:100% !important;
        display: flex;
        justify-content: center;
    }
}
@media (max-width:260px) {
    .constrols__item {
        margin-right: 0;
    }
    .home-page__title {
        margin-bottom: 10vh;
    }
} 
</style>
