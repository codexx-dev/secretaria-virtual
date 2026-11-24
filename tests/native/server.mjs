// Sobe o nat_comp numa pasta temporaria para os testes, sem instalar.
//
// O executavel vem de build/ (compile antes: cmake --build build) ou da
// variavel NAT_COMP_EXE. A configuracao, os dados e o log.txt ficam na pasta
// temporaria, que e apagada no stop().

import { spawn } from "node:child_process";
import { existsSync, readFileSync } from "node:fs";
import { mkdir, mkdtemp, rm, writeFile } from "node:fs/promises";
import { createServer } from "node:net";
import { tmpdir } from "node:os";
import { join, resolve } from "node:path";

// Origens aceitas no build: as de native/origins.txt (mesmas regras do CMake).
export const BUILD_ORIGINS = readFileSync(resolve("native/origins.txt"), "utf8")
  .split(/\r?\n/)
  .map((line) => line.trim().toLowerCase().replace(/\/+$/, ""))
  .filter((line) => line && !line.startsWith("#"));

const EXE = process.env.NAT_COMP_EXE ?? resolve("build/nat_comp_win10.exe");

function freePort() {
  return new Promise((ok) => {
    const srv = createServer().listen(0, "127.0.0.1", () => {
      const { port } = srv.address();
      srv.close(() => ok(port));
    });
  });
}

// editConfig: recebe o config.json padrao do teste e pode altera-lo antes de gravar.
export async function startServer({ editConfig = () => {} } = {}) {
  if (!existsSync(EXE)) throw new Error(`Executavel nao encontrado: ${EXE}. Compile com: cmake --build build`);

  const root = await mkdtemp(join(tmpdir(), "nat-comp-test-"));
  const installDir = join(root, "appdata", "SecretariaVirtual");
  const dataDir = join(root, "dados");
  await mkdir(installDir, { recursive: true });
  await mkdir(dataDir, { recursive: true });

  const port = await freePort();
  const config = { dados: dataDir, porta: port, usuario: "teste" };
  editConfig(config);
  const configFile = join(installDir, "config.json");
  await writeFile(configFile, JSON.stringify(config));

  const proc = spawn(EXE, ["--no-install"], {
    env: { ...process.env, APPDATA: join(root, "appdata") },
    stdio: "ignore",
  });
  const exited = new Promise((r) => proc.once("exit", r));

  const stop = async () => {
    if (proc.exitCode === null) proc.kill();
    await exited;
    await rm(root, { recursive: true, force: true });
  };

  const url = `http://127.0.0.1:${port}`;
  for (let i = 0; ; i++) {
    try {
      if ((await fetch(`${url}/api/ping`)).ok) break;
    } catch {}
    if (proc.exitCode !== null || i === 50) {
      await stop();
      throw new Error("O servidor nao subiu.");
    }
    await new Promise((r) => setTimeout(r, 100));
  }

  return { url, dataDir, configFile, logFile: join(installDir, "log.txt"), stop };
}
