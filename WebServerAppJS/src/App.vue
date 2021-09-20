<template>
  <section class="container" id="app">
    <router-view/>
  </section>
</template>

<style>
:root {
    --white:#fff;
    --blue:rgb(117,164,255);
    --black:#000000;
    --light-green:#A5FF96;
    --red: #FF2929;
    --gray: rgba(196, 196, 196, 0.3);
}
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    outline: none;
    font-family: "Gayathri", sans-serif;
    user-select: none;
    outline: none;
    -webkit-tap-highlight-color:rgba(255,255,255,0);
    -webkit-tap-highlight-color: transparent;
    overflow-wrap: break-word;
}
body {
    height: 100vh;
    position: relative;
}
::-webkit-scrollbar {
    width: 0;
}
.container {
    width: 40%;
    margin: 0 auto;
    padding: 3%;
    position: relative;
}
.title-text {
    font-size: 2rem;
    font-weight: 400;
}
.title {
    font-size: 1rem;
    margin-bottom: 1rem;
}
/*Устройства*/
.navigation {
    display: flex;
    flex-direction: row;
    align-items: center;
    margin-bottom: 1rem;
}
.navigation-arrow svg path{
    fill: #C2C2C2;
}
.navigation-arrow {
    display: flex;
    flex-direction: row;
    align-items: center;
    margin-right: 1rem;
    cursor: pointer;
}
.navigation-icon path {
    fill: #C2C2C2;
}
@media(max-width:1280px) {
    .container {
        width: 60%;
    }
    .devices__text {
        font-size: 1rem;
    }
    .devices__item {
        padding: 1rem;
    }
    .form-edit {
        width: 50%;
    }
}
@media (max-width:730px) {
    .container {
        width: 100%;
    }
    .form-edit {
        width: 60%;
    }
}
@media(max-width:480px) {
    .form-edit {
        width: 90%;
    } 
    .devices__icon {
        width: 25px;
        height: 25px;
    }
    .devices__col-1-2 {
        width: 38%;
    }
}
</style>
