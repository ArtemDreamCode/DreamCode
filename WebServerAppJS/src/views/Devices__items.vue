<template>
    <article class = "devices__box">
        <div v-for = "device in devices.slice(from__count,to__count)">
            <div v-bind:key="device"  @click="devices_active(device,$event)" class="devices__item" v-bind:class="{'device--active': device.state == 'off' ? false : true}">
                <svg class = "devices__icon" width="35" height="35" viewBox="0 0 52 52" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M25.9091 35.0728C23.4408 35.0728 21.0736 34.1194 19.3282 32.4225C17.5829 30.7255 16.6023 28.4239 16.6023 26.0241C16.6023 23.6242 17.5829 21.3226 19.3282 19.6257C21.0736 17.9287 23.4408 16.9754 25.9091 16.9754C28.3775 16.9754 30.7447 17.9287 32.4901 19.6257C34.2354 21.3226 35.216 23.6242 35.216 26.0241C35.216 28.4239 34.2354 30.7255 32.4901 32.4225C30.7447 34.1194 28.3775 35.0728 25.9091 35.0728ZM45.6662 28.5319C45.7725 27.7046 45.8523 26.8772 45.8523 26.0241C45.8523 25.1709 45.7725 24.3178 45.6662 23.4387L51.2769 19.2246C51.7821 18.8368 51.915 18.1388 51.596 17.57L46.2778 8.62473C45.9587 8.05595 45.2407 7.82327 44.6557 8.05595L38.0346 10.6413C36.6519 9.63301 35.216 8.75399 33.5407 8.10766L32.5569 1.2565C32.4505 0.636016 31.8921 0.170654 31.2273 0.170654H20.591C19.9262 0.170654 19.3678 0.636016 19.2614 1.2565L18.2775 8.10766C16.6023 8.75399 15.1664 9.63301 13.7837 10.6413L7.16255 8.05595C6.57755 7.82327 5.8596 8.05595 5.5405 8.62473L0.222323 17.57C-0.123359 18.1388 0.0361868 18.8368 0.541414 19.2246L6.1521 23.4387C6.04573 24.3178 5.96596 25.1709 5.96596 26.0241C5.96596 26.8772 6.04573 27.7046 6.1521 28.5319L0.541414 32.8235C0.0361868 33.2113 -0.123359 33.9094 0.222323 34.4782L5.5405 43.4234C5.8596 43.9922 6.57755 44.199 7.16255 43.9922L13.7837 41.381C15.1664 42.4152 16.6023 43.2942 18.2775 43.9405L19.2614 50.7917C19.3678 51.4122 19.9262 51.8775 20.591 51.8775H31.2273C31.8921 51.8775 32.4505 51.4122 32.5569 50.7917L33.5407 43.9405C35.216 43.2683 36.6519 42.4152 38.0346 41.381L44.6557 43.9922C45.2407 44.199 45.9587 43.9922 46.2778 43.4234L51.596 34.4782C51.915 33.9094 51.7821 33.2113 51.2769 32.8235L45.6662 28.5319Z" fill="#75A4FF"/>
                </svg>
                <h2 class = "devices__text">
                    {{ device.name }}
                </h2>
            </div>
        <div>
    </article>
</template>

<script>
import { io } from "socket.io-client";
const socket = io("http://lycoris.su:3000/");
export default {
    name: 'DevicesBox',
    props:  ['fromCount','toCount', 'devices'],
    methods: {
        devices_active (device, event) {
		let item = event.path.find(elem => (elem.classList.contains("devices__item")) ? elem : false)
		item.classList.toggle('device--active');
		var dev = item.classList.contains("device--active") ? {"ip": device.ip, "turn": "on"} : {"ip": device.ip, "turn": "off"}
                socket.emit("relay", dev)
        }
    },
    mounted() {
        socket.on("devices_old", devices => {
            this.devices = devices;
        })
    }
}
</script>

<style scoped>
.devices__box {
    display: flex;
    flex-direction: row;
    flex-flow: row wrap;
    height: 100%;
    justify-content: center;
    width: 50%;
    margin: 0 auto;
}
.devices__text {
    font-weight: 400;
    font-size: 1.3rem;
}
.device--active {
    background: var(--light-green);
}
.devices__icon {
    margin-right: 1rem;
}
.box--active {
    display: flex !important;
}
.devices__item {
    display: flex;
    flex-direction: row;
    align-items: center;
    justify-content: center;
    padding: 1.5rem;
    border-radius: 10px;
    border:2px solid var(--gray);
    cursor: pointer;
    margin-bottom: 0.7rem;
    margin-right: 0.5rem;
}
.devices__box:nth-child(1n) {
    background:black
}
.devices__box:nth-child(2n) {
    background:red
}
.devices__box:nth-child(3n) {
    background:blue
}
.devices__box:nth-child(4n) {
    background:gray
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
@media(max-width: 1100px) {
    .devices__box {
        width: 80%;
    }
}
@media(max-width: 480px) {
    .devices__box {
        width: 80%;
    }
}
</style>
