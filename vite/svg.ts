/// <reference types="node" />
import { readFile } from "node:fs/promises";
import type { Plugin } from "vite";

// Importa um .svg como componente Solid: import Icon from "~/assets/x.svg".
// O modulo ganha o sufixo .jsx para o plugin do Solid compilar. Substitui o
// vite-plugin-solid-svg, que so funciona com o Solid 1.
const SUFFIX = ".svg.jsx";

export function svgComponent(): Plugin {
  return {
    name: "svg-component",
    enforce: "pre",
    async resolveId(source, importer, options) {
      if (!source.endsWith(".svg")) return;
      const file = await this.resolve(source, importer, {
        ...options,
        skipSelf: true,
      });
      if (file) return `${file.id}.jsx`;
    },
    async load(id) {
      if (!id.endsWith(SUFFIX)) return;
      const file = id.slice(0, -".jsx".length);
      this.addWatchFile(file);
      const svg = await readFile(file, "utf8");
      // {...props} no fim: class e outros atributos sobrescrevem os do arquivo.
      const jsx = svg
        .slice(svg.indexOf("<svg"))
        .replace(/<svg([^>]*?)(\/?)>/, "<svg$1 {...props}$2>");
      return `export default (props) => (${jsx.trim()});\n`;
    },
  };
}
