<template>
    <div>
        <div class="scroll-table">
            <table>
                <thead>
                    <tr>
                        <th v-for="item in tHead" :key="item.id">{{item}}</th>
                    </tr>
                </thead>
            </table>	
            <div v-if ="tBody.length == 0" class="edit_box" v-show="edit_display">
                <div class="form-edit">
                    <div @click="close_edit" class="box__close">
                        <svg class = "js-close-edit" width="14" height="14" viewBox="0 0 14 14" fill="none" xmlns="http://www.w3.org/2000/svg">
                            <path d="M12.59 0L7 5.59L1.41 0L0 1.41L5.59 7L0 12.59L1.41 14L7 8.41L12.59 14L14 12.59L8.41 7L14 1.41L12.59 0Z" fill="#2E3A59"/>
                        </svg>
                    </div>
                    <div class="text">
                        <p>Нет новых устройств!</p>
                    </div>
                </div>
            </div>
            <div v-if="tBody.length > 0" class="scroll-table-body">
                <table>
                    <tbody>
                        <tr v-for="row in tBody" :key="row.id">
                            <td v-for="item in row" :key="item.id">{{item}}</td>
                            <td>
                                <div class="edit">
                                    <svg @click="curr_ip = $event.currentTarget.id.split(',')[1];curr_name = $event.currentTarget.id.split(',')[0];open_edit()" :id="row" width="14" height="14" viewBox="0 0 38 38" fill="none" xmlns="http://www.w3.org/2000/svg">
                                        <path d="M37.1458 8.66667C37.9583 7.85416 37.9583 6.5 37.1458 5.72917L32.2708 0.854164C31.5 0.0416641 30.1458 0.0416641 29.3333 0.854164L25.5 4.66666L33.3125 12.4792L37.1458 8.66667ZM0.25 29.9375V37.75H8.0625L31.1042 14.6875L23.2917 6.875L0.25 29.9375Z" fill="black"/>
                                    </svg>
                                    <div class="edit_box" v-show="edit_display">
                                        <div class="form-edit">
                                            <div @click="close_edit" class="box__close">
                                                <svg class = "js-close-edit" width="14" height="14" viewBox="0 0 14 14" fill="none" xmlns="http://www.w3.org/2000/svg">
                                                    <path d="M12.59 0L7 5.59L1.41 0L0 1.41L5.59 7L0 12.59L1.41 14L7 8.41L12.59 14L14 12.59L8.41 7L14 1.41L12.59 0Z" fill="#2E3A59"/>
                                                </svg>
                                            </div>
                                            <input @click="keyboard_open" type="text" v-model="input_data" class = "form_input input" name = "device-name" :placeholder="curr_name" />
                                            <div v-if="reset == true" @click="resetDevice" :id="row[1]" class="button button-edit">Сбросить устройство</div>
                                            <button @click="updateDevice" :id="row[1]" class="button" type="submit">Сохранить</button>
                                        </div>
                                        <div v-show="keyboard_display" class="simple-keyboard"></div>
                                    </div>
                                </div>
                            </td>
                        </tr>
                    </tbody>
                </table>
            </div>	
        </div>
    </div>
</template>

<script>
import Keyboard from 'simple-keyboard';
import 'simple-keyboard/build/css/index.css';
import { io } from "socket.io-client";
const socket = io("http://localhost:3000");

export default {
    name: "Table",
    props: ['tHead', 'tBody', 'reset'],
    data () {
        return {
            input_data: '',
            edit_display: false,
            keyboard_display: false,
			curr_ip: 0,
			curr_name: ""
        }
    },
    methods: {
        resetDevice() {
            var device = {"ip":this.curr_ip};
            socket.emit("reset",device);
			this.curr_ip=0;
			this.input_data = '';
            this.edit_display = false;
        },
        updateDevice() {
			if (this.input_data != '') {
            var device = {"ip":this.curr_ip, "name":this.input_data}
            socket.emit("ChangeName",device);
			this.curr_ip=0;
			this.input_data = '';
            this.edit_display = false;
			} else {
				alert("Введите имя")
			}
        },
        open_edit () {
			console.log(this.curr_ip)
            this.edit_display = true;
        },
        close_edit () {
            this.edit_display = false;
        },
        clear_input () {
            this.input_data = '';
        },
        keyboard_open () {
            if(this.keyboard_display == false) {
                this.keyboard_display = true
            } else {
                this.keyboard_display = false
            }
        },
        onChange(input){
            this.input_data = input
        }
    },
	mounted() {
        setTimeout(() => {
            const keyboard = new Keyboard({
                onChange: input => this.onChange(input),
            });
            keyboard;
        },  1000)
    }
}
</script>

<style scoped>
.table {
    width: 100%;
    border-collapse: collapse;
    font-size: 1rem;
    color:var(--black);
    text-align: left;
}
/*Keybaords*/
.simple-keyboard  {
    z-index:9999;
    margin-bottom: -55%;
}
.edit {
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
}
.scroll-table-body {
	border-bottom: 1px solid #eee;
}
.scroll-table table thead tr th:last-child {
    width: 5px !important;
}
.scroll-table-body table tbody tr td:last-child {
    width: 30px !important;
}
.scroll-table table {
	width:100%;
	table-layout: fixed;
	border: none;
}
.scroll-table thead th {
	font-weight: bold;
	text-align: left;
	border: none;
	padding: 10px 15px;
	background: var(--blue);
	font-size: 14px;
    color:var(--white)
}
.scroll-table tbody td {
	text-align: left;
	padding: 5px;
	font-size: 0.875rem;
	vertical-align: top;
}
.table__state {
    display: flex;
    flex-direction: row;
    justify-content: space-between;
}
.edit_box {
    position: absolute;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    display: flex;
    align-items: center;
    justify-content: center;
    border-radius: 10px;
}
.form-edit {
    position: absolute;
    display: flex;
    flex-direction: column;
    padding: 1rem;
    background: var(--white);
    width: 60%;
    z-index: 999;
    border-radius: 5px;
    border:2px var(--blue) solid;
}
.form_input {
    margin-bottom: 1rem;
    border:2px solid var(--blue);    
    outline: none;
    padding: 0.5rem;
    border-radius:5px;
}
.button {
    padding: 0.5rem;
    border: 2px solid var(--blue);
    border-radius: 5px;
    background:none;
    cursor: pointer;
}
.button-edit {
    width:100%;
    margin-bottom: 1rem;
    background: none;
    text-align: center;
    border:2px solid var(--blue);
}
.box__close {
    display: flex;
    margin-bottom: 1rem;
    justify-content: flex-end;
}
@media(max-width:1280px) {
    .form-edit {
        width: 50%;
    }
}
@media (max-width:730px) {
    .form-edit {
        width: 60%;
    }
}
@media(max-width:600px) {
    .scroll-table tbody td {
        font-size: 0.9rem;
    }
}
@media(max-width:480px) {
    .form-edit {
        width: 90%;
        margin-bottom: 10%;
    } 
}
</style>
