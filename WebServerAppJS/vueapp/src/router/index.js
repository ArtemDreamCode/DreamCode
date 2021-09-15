import Vue from 'vue'
import VueRouter from 'vue-router'
import Menu from '@/views/Menu.vue'
import Settings from '@/views/Settings.vue'
import Devices from '@/views/Devices.vue'
import New_devices from '@/views/New_devices.vue'
Vue.use(VueRouter)

const routes = [
  {
    path: '/',
    name: 'Menu',
    component: Menu
  },
  {
    path: '/settings',
    name: 'Settings',
    component: Settings
  },
  {
    path: '/devices',
    name: 'Devices',
    component: Devices
  },
  {
    path: '/new-devices',
    name: 'New_devices',
    component: New_devices
  }
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

export default router
