import path from "node:path";
import fs from "node:fs";
import { fileURLToPath } from "node:url";

import { BASENAME, routes } from "./src/routes.jsx";
import { createStaticHandler, createStaticRouter, StaticRouterProvider } from "react-router-dom";
import { jsx } from "react/jsx-runtime";
import { renderToString } from "react-dom/server";

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const distDir = path.resolve(__dirname, "dist");

// handler estático que permite pré-render
const handler = createStaticHandler(routes);

async function renderRoute(url) {
  const context = await handler.query(
    new Request("http://example.com" + url)
  );

  const router = createStaticRouter(routes, context, {basename: BASENAME});
  const app = (jsx(StaticRouterProvider, {router, context}));

  return renderToString(app);
}

// rotas que você quer pré-renderizar
const pages = [
  '/',
  '/cracha',
  '/imprimir/cracha',
  '/sobre',
  './em-progresso',
  '/__404__'
];

for (const url of pages) {
  const html = fs.readFileSync(path.join(distDir, "index.html"), "utf-8");
  const rendered = await renderRoute(url);

  const finalHtml = html.replace(
    '<div id="root"></div>',
    `<div id="root">${rendered}</div>`
  );

  const outputPath =
    url === "/"
      ? path.join(distDir, "index.html")
      : path.join(distDir, url, "index.html");

  fs.mkdirSync(path.dirname(outputPath), { recursive: true });
  fs.writeFileSync(outputPath, finalHtml);
  console.log("✅ página gerada:", url);
}
