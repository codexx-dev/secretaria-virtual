// Testes da API do nat_comp (componente nativo).

import assert from "node:assert/strict";
import { mkdir, readFile, writeFile } from "node:fs/promises";
import { join } from "node:path";
import { after, before, describe, test } from "node:test";

import { BUILD_ORIGINS, startServer } from "./server.mjs";

const SRC = "TRANSPORTE ESCOLAR/ALUNOS";

let server;
before(async () => {
  server = await startServer();
});
after(() => server.stop());

function call(method, path, body) {
  return fetch(server.url + path, {
    method,
    headers: { "Content-Type": "application/json" },
    body: body && JSON.stringify(body),
  });
}

const getTable = () => call("GET", `/api/dados?src=${encodeURIComponent(SRC)}`);

describe("/api/dados", () => {
  before(async () => {
    await mkdir(join(server.dataDir, "TRANSPORTE ESCOLAR"));
    await writeFile(join(server.dataDir, `${SRC}.csv`), "﻿nome;mãe\r\nAna;Maria\r\n");
  });

  test("GET devolve a tabela em JSON", async () => {
    const res = await getTable();
    assert.equal(res.status, 200);
    assert.deepEqual(await res.json(), [{ nome: "Ana", mãe: "Maria" }]);
  });

  test("POST insere e recusa linha igual (espacos nas pontas nao contam)", async () => {
    assert.equal((await call("POST", "/api/dados", { src: SRC, new: { nome: "Bia", mãe: "Joana" } })).status, 201);
    assert.equal((await call("POST", "/api/dados", { src: SRC, new: { mãe: " Joana ", nome: "Bia" } })).status, 409);
  });

  test("PUT troca a linha e recusa linha antiga que nao existe mais", async () => {
    const old = { nome: "Ana", mãe: "Maria" };
    assert.equal((await call("PUT", "/api/dados", { src: SRC, old, new: { nome: "Ana Maria", mãe: "Maria" } })).status, 204);
    assert.equal((await call("PUT", "/api/dados", { src: SRC, old, new: { nome: "X", mãe: "Y" } })).status, 404);
  });

  test("DELETE apaga a linha e, com delete-all, a tabela inteira", async () => {
    assert.equal((await call("DELETE", "/api/dados", { src: SRC, old: { nome: "Bia", mãe: "Joana" } })).status, 204);
    assert.deepEqual(await (await getTable()).json(), [{ nome: "Ana Maria", mãe: "Maria" }]);

    assert.equal((await call("DELETE", "/api/dados", { src: SRC, "delete-all": true })).status, 204);
    assert.equal((await getTable()).status, 404);
  });

  test("recusa src fora da pasta de dados", async () => {
    const res = await call("GET", "/api/dados?src=../../x");
    assert.equal(res.status, 400);
    assert.ok((await res.json()).err);
  });

  test("registra as respostas no log.txt, com a descricao do erro", async () => {
    const log = await readFile(server.logFile, "utf8");
    assert.match(log, /\[INFO\] POST \/api\/dados -> 201/);
    assert.match(log, /\[ERRO\] POST \/api\/dados -> 409 \{"err":"Ja existe uma linha igual\."\}/);
  });
});

describe("/api/configuracao", () => {
  test("GET devolve a configuracao atual", async () => {
    const config = await (await call("GET", "/api/configuracao")).json();
    assert.deepEqual(Object.keys(config).sort(), ["data", "port", "user"]);
    assert.equal(config.user, "teste");
  });

  test("POST recusa valor invalido e grava o valido no config.json", async () => {
    assert.equal((await call("POST", "/api/configuracao", { port: 70000 })).status, 400);

    const res = await call("POST", "/api/configuracao", { user: "Secretária 1" });
    assert.equal(res.status, 200);
    assert.equal((await res.json()).user, "Secretária 1");
    assert.equal(JSON.parse(await readFile(server.configFile, "utf8")).usuario, "Secretária 1");
  });

  test("POST so aceita pasta de dados dentro da pasta do usuario (ou na rede)", async () => {
    const outside = process.env.SystemRoot; // C:\Windows
    assert.equal((await call("POST", "/api/configuracao", { data: outside })).status, 400);

    const inside = join(server.dataDir, "nova pasta");
    await mkdir(inside);
    assert.equal((await call("POST", "/api/configuracao", { data: inside })).status, 200);
  });
});

describe("CORS", () => {
  const ping = (origin, method = "GET") =>
    fetch(`${server.url}/api/ping`, { method, headers: { Origin: origin, "Access-Control-Request-Method": "PUT" } });

  test("aceita as origens do origins.txt e localhost em qualquer porta, inclusive no preflight", async () => {
    for (const origin of [...BUILD_ORIGINS, "http://localhost:5173", "http://127.0.0.1"]) {
      const res = await ping(origin);
      assert.equal(res.status, 200, origin);
      assert.equal(res.headers.get("access-control-allow-origin"), origin);
    }

    const preflight = await ping("http://localhost:3000", "OPTIONS");
    assert.equal(preflight.status, 204);
    assert.match(preflight.headers.get("access-control-allow-methods"), /PUT/);
  });

  test("recusa outras origens, inclusive as parecidas com localhost", async () => {
    for (const origin of ["https://outro-site.example", "http://localhost.outro-site.example"])
      assert.equal((await ping(origin)).status, 403, origin);
  });
});

describe("config.json na inicializacao", () => {
  test("opcao ausente e completada com o padrao e opcao antiga e removida", async () => {
    const s = await startServer({
      editConfig: (c) => {
        delete c.usuario;
        c.origins = ["https://outro-site.example"]; // saiu do config.json
      },
    });
    try {
      const saved = JSON.parse(await readFile(s.configFile, "utf8"));
      assert.equal(saved.usuario, "default");
      assert.equal(saved.origins, undefined);
    } finally {
      await s.stop();
    }
  });
});
