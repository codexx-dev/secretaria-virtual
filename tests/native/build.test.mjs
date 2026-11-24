// Testes do build do nat_comp (CMake). Precisa do cmake no PATH (ou na variavel CMAKE).

import assert from "node:assert/strict";
import { spawnSync } from "node:child_process";
import { mkdtempSync, rmSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { test } from "node:test";

test("cmake falha explicando o motivo quando falta o arquivo de origens", () => {
  const dir = mkdtempSync(join(tmpdir(), "nat-comp-cmake-"));
  try {
    const res = spawnSync(process.env.CMAKE ?? "cmake", ["-S", "native", "-B", dir, `-DORIGINS_FILE=${join(dir, "nao-existe.txt")}`], {
      encoding: "utf8",
    });
    assert.ifError(res.error);
    assert.notEqual(res.status, 0);
    assert.match(res.stderr, /Arquivo de origens nao encontrado/);
    assert.match(res.stderr, /origins\.example\.txt/);
  } finally {
    rmSync(dir, { recursive: true, force: true });
  }
});
