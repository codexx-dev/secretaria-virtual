import { defineConfig } from "vite";
import { nitro } from "nitro/vite";
import { solidStart } from "@solidjs/start/config";
import tailwindcss from "@tailwindcss/vite";
import solidSvg from 'vite-plugin-solid-svg';

export default defineConfig({
  plugins: [
    solidStart(),
    tailwindcss(),
    nitro(),
    solidSvg()
  ],
  nitro: {
    preset: "static",
    prerender: {
      crawlLinks: true,
      failOnError: true,
      routes: ["/", "/404.html"]
    },
  },
});
