<template>
    <div v-if = "message_display" class="padding message-container">
        <div class="form-edit">
            <div @click="close_message" class="box__close">
                <span class = "message__title">{{title}}</span>
                <svg class = "js-close-edit" width="14" height="14" viewBox="0 0 14 14" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M12.59 0L7 5.59L1.41 0L0 1.41L5.59 7L0 12.59L1.41 14L7 8.41L12.59 14L14 12.59L8.41 7L14 1.41L12.59 0Z" fill="#2E3A59"/>
                </svg>
            </div>
            <div class="message__body">
                <span>{{text}}</span>    
            </div>
            <div class="message__button-box">
                <button @click = "close_message" class="button" type="submit">OK</button> 
            </div>
        </div>
    </div>
</template>
<script>
export default {
    name: "Message",
    props:  ['text', 'title'],
    data () {
        return {
            message_display : true
        }
    },
    methods: {
        close_message () {
            this.message_display = false;
        },
    }
}
</script>

<style scoped>

.form-edit {
    position: absolute;
    display: flex;
    flex-direction: column;
    padding: 1rem;
    background: #F1F1F1;
    width: 80%;
    z-index: 999;
    border-radius: 5px;
    border:2px var(--gray) solid;
}
.message-container {
    position: absolute;
    top:0;
    left:0;
    width:100%;
    height:100vh;
    display: flex;
    justify-content: center;
    align-items: center;
}
.form-edit {
    border:2px solid var(--blue);
    background : var(--white);
    border-radius: 5px;
}
.message__title {
    
}
.message__body {
    padding:1rem;
    margin-bottom: 1rem;
}
.message__button-box {
    display: flex;
    flex-direction:row;
    justify-content: center;
}
.button {
    padding: 0.5rem;
    width:30%;
    cursor: pointer;
    border:none;
    transition: all ease 0.3s;
    background:none;
    border:2px solid var(--blue);
    border-radius:5px;
}
.button:hover {
    color:var(--blue)
}
.box__close {
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    margin-bottom: 1rem;
    cursor: pointer;
}
.box__close:hover svg path {
    fill: var(--blue);
    transition:all ease 0.3s;
}
</style>