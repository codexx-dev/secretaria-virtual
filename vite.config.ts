import { defineConfig } from "vite";
import { nitro } from "nitro/vite";
import solid from "@solidjs/vite-plugin";
import { fileRoutes } from "filesystem-routing/vite";
import tailwindcss from "@tailwindcss/vite";
import { svgComponent } from "./vite/svg.ts";

export default defineConfig({
  plugins: [
    svgComponent(),
    fileRoutes(),
    // "extensions" faz o Solid compilar tambem os modulos de rota e de svg,
    // cujos ids nao sao um caminho de arquivo comum.
    solid({ start: true, ssr: true, extensions: [".jsx", ".tsx"] }),
    tailwindcss(),
    nitro(),
  ],
  resolve: {
    tsconfigPaths: true,
  },
  nitro: {
    preset: "static",
    prerender: {
      crawlLinks: true,
      failOnError: true,
      routes: ["/", "/404.html"]
    },
    hooks: {
      // O crawler do Nitro nao enxerga o href de um <a> que vem depois do
      // _hk=123 (sem aspas) do Solid 2, e a pagina ficaria fora do build.
      "prerender:generate"(route) {
        if (route.fileName?.endsWith(".html"))
          route.contents = route.contents?.replace(/ _hk=(\w+)/g, " _hk=\"$1\"");
      },
    },
  },
});
