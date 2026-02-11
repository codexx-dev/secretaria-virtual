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

  const router = createStaticRouter(routes, context);
  const app = (jsx(StaticRouterProvider, {router, context}));

  return {rendered: renderToString(app), context: context};
}

// rotas que você quer pré-renderizar
const pages = [
  '/',
  '/cracha',
  '/imprimir/cracha',
  '/sobre',
  '/em-progresso',
  '/__404__'
];

const template = fs.readFileSync(
  path.join(distDir, "index.html"),
  "utf-8"
);

for (const url of pages) {
  const {rendered, context} = await renderRoute(url);

  const finalHtml = template.replace(
    '<div id="root"></div>',
    `<div id="root">${rendered}</div>`
  )
  .replace(
    '</body>',
    `<script>window.__STATIC_ROUTER_DATA__ = ${JSON.stringify(context)};</script></body>`
  );

  const outputPath = (url === "/")?
  path.join(distDir, "index.html") :
  path.join(distDir, url, "index.html");

  fs.mkdirSync(path.dirname(outputPath), { recursive: true });
  fs.writeFileSync(outputPath, finalHtml);

  console.log("✅ página gerada", url);
  console.log(`URL: http://example.com/${BASENAME}${url}`);
  //console.log("Resolvida:", context.location.pathname);
}
