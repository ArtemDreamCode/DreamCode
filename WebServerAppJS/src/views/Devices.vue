<template>
    <div class = "devices">
        <div class="navigation">
            <a href = "/" class="navigation-arrow">
                <svg width="12" height="22" viewBox="0 0 8 14" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M0.288025 7.00001L6.29802 13.01L7.71202 11.596L3.11202 6.99601L7.71202 2.39601L6.29802 0.990005L0.288025 7.00001Z" fill="#2E3A59"/>
                </svg>
            </a>
            <svg class="navigation-icon"  width="25" height="29" viewBox="0 0 118 100" fill="none" xmlns="http://www.w3.org/2000/svg">
                <path d="M59 16.1917L88.1667 42.4417V88H76.5V53H41.5V88H29.8333V42.4417L59 16.1917ZM59 0.5L0.666656 53H18.1667V99.6667H53.1667V64.6667H64.8333V99.6667H99.8333V53H117.333L59 0.5Z" fill="#75A4FF"/>
            </svg>
        </div>
        <DevicesBox/>
    </div>
</template>

<script>
import DevicesBox from "@/components/DevicesBox"
export default {
    name: 'Devices',
    components: {
      DevicesBox
    }
}
</script>

<style scoped>
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
.devices__item:nth-child(2n) {
    margin-right: 0;
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
</style>
