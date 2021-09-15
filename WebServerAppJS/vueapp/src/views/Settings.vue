<template>
    <div class = "settings">
        <div class="navigation">
            <a href = "/" class="navigation-arrow">
                <svg width="12" height="22" viewBox="0 0 8 14" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M0.288025 7.00001L6.29802 13.01L7.71202 11.596L3.11202 6.99601L7.71202 2.39601L6.29802 0.990005L0.288025 7.00001Z" fill="#2E3A59"/>
                </svg>
            </a>
            <svg class="navigation-icon" width="24" height="24" viewBox="0 0 98 100" fill="none" xmlns="http://www.w3.org/2000/svg">
                <path d="M49 67.5C44.3588 67.5 39.9076 65.6562 36.6257 62.3744C33.3438 59.0925 31.5 54.6413 31.5 50C31.5 45.3587 33.3438 40.9075 36.6257 37.6256C39.9076 34.3437 44.3588 32.5 49 32.5C53.6413 32.5 58.0925 34.3437 61.3744 37.6256C64.6563 40.9075 66.5 45.3587 66.5 50C66.5 54.6413 64.6563 59.0925 61.3744 62.3744C58.0925 65.6562 53.6413 67.5 49 67.5ZM86.15 54.85C86.35 53.25 86.5 51.65 86.5 50C86.5 48.35 86.35 46.7 86.15 45L96.7001 36.85C97.65 36.1 97.9 34.75 97.3 33.65L87.3 16.35C86.7 15.25 85.35 14.8 84.25 15.25L71.8 20.25C69.2 18.3 66.5 16.6 63.35 15.35L61.5 2.1C61.3 0.899997 60.25 -3.8147e-06 59 -3.8147e-06H39C37.75 -3.8147e-06 36.7 0.899997 36.5 2.1L34.65 15.35C31.5 16.6 28.8 18.3 26.2 20.25L13.75 15.25C12.6501 14.8 11.3001 15.25 10.7001 16.35L0.700051 33.65C0.0500514 34.75 0.350052 36.1 1.30005 36.85L11.8501 45C11.6501 46.7 11.5001 48.35 11.5001 50C11.5001 51.65 11.6501 53.25 11.8501 54.85L1.30005 63.15C0.350052 63.9 0.0500514 65.25 0.700051 66.35L10.7001 83.65C11.3001 84.75 12.6501 85.15 13.75 84.75L26.2 79.7C28.8 81.7 31.5 83.4 34.65 84.65L36.5 97.9C36.7 99.1 37.75 100 39 100H59C60.25 100 61.3 99.1 61.5 97.9L63.35 84.65C66.5 83.35 69.2 81.7 71.8 79.7L84.25 84.75C85.35 85.15 86.7 84.75 87.3 83.65L97.3 66.35C97.9 65.25 97.65 63.9 96.7001 63.15L86.15 54.85Z" fill="#75A4FF"/>
            </svg>
        </div>
        <div class="setting__devices-button">
            <a href = "/settings" class="devices-button__item button--active">
                <span class = "button-text">Подключенные устройства</span>
            </a>
            <a v-if = "new_devices > 0" href = "/new-devices" class="devices-button__item">
                <div class="message-count">
                    <span class = "message-count__number">{{new_devices}}</span>
                </div>
                <span href = "/new-devices" class = "button-text">Новые устройства</span>
            </a>
        </div>
        <Table :tBody = "tableBody" :tHead = "tableHead" :reset = "reset">
	</Table>
    </div>
</template>

<script>
import Table from "../components/table.vue";
import { io } from "socket.io-client";

export default {
  name: 'Settings',
  data () {
      return {
        tableHead : ['Устройства', 'IP', 'Состояние',''],
        tableBody: [],
        reset : true,
		new_devices: 0
      }
  },
  components: {
      Table
  },
  created() {
		if (localStorage.devices_old) {
			var devices = JSON.parse(localStorage.getItem("devices_old"));
            if (devices.length == 0){
				window.location.href="/new-devices"
			}
		}
  },
  mounted() {
		if (localStorage.devices_old) {
			this.tableBody = [];
			var devices = JSON.parse(localStorage.getItem("devices_old"));
            devices.forEach((device, index) => {
                this.tableBody[index] = [device.name, device.ip, device.state];
            })
		}
		if (localStorage.CountNewDev) {
			this.new_devices = localStorage.CountNewDev;
		}
		const socket = io("http://localhost:3000/");
		socket.on("devices_old", devices => { 
			this.tableBody = [];
            devices.forEach((device, index) => {
                this.tableBody[index] = [device.name, device.ip, device.state];
            })
			if (this.tableBody.length == 0){
				window.location.href = "/new-devices";
			}
			localStorage.setItem("devices_old", JSON.stringify(devices))
						
        });
        socket.on("CountNewDev", devices => {
			this.new_devices = devices
            if (devices != 0 && this.tableBody.length == 0) {
                window.location.href="/new-devices"
            }
			localStorage.setItem("CountNewDev", devices)

        })
	}
}
</script>

<style scoped>
.setting__devices-button {
    display: flex;
    flex-direction: row;
    margin-bottom: 0.8rem;
}
.devices-button__item {
    position: relative;
    padding: 0.7rem;
    text-align: center;
    border-radius: 5px;
    margin-right: 1.2rem;
    border:1px solid var(--black);
    cursor: pointer;
    text-decoration:none;
    color:var(--black);
}
.devices-button__item:nth-child(2n) {
    margin-right: 0;
}
.devices-button__item .message-count {
    top: -28%;
    right: -8%;
    width: 30px;
    height: 30px;
}
.devices-button__item .message-count__number {
    font-size: 1rem;
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
.button--active{
    background: var(--blue);
    border:none;
    color:var(--white);
}
.button-text {
    text-decoration:none;
}
@media(max-width:480px) {
    .devices-button__item .message-count {
        top: -15%;
        right: -4%;
    }
}
</style>
