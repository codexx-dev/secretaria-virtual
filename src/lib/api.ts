// Cliente da API do componente nativo (nat_comp), que roda no PC do usuario.
// O site e estatico: a porta do componente fica no localStorage do navegador.

export type Row = Record<string, string>;

export type Config = {
  data: string;
  port: number;
  user: string;
};

export const DEFAULT_PORT = 8080;

const PORT_KEY = "nat_comp.porta";

export function getPort(): number {
  if (typeof localStorage === "undefined") return DEFAULT_PORT;
  const port = Number(localStorage.getItem(PORT_KEY));
  return Number.isInteger(port) && port > 0 && port <= 65535
    ? port
    : DEFAULT_PORT;
}

export function setPort(port: number) {
  localStorage.setItem(PORT_KEY, String(port));
}

/** O servidor respondeu com erro; `status` e o codigo HTTP e `message` o texto de {"err"}. */
export class ApiError extends Error {
  constructor(
    readonly status: number,
    message: string,
  ) {
    super(message);
  }
}

/** Nao foi possivel falar com o componente nativo (desligado, porta errada, CORS). */
export class OfflineError extends Error {
  constructor() {
    super("Não foi possível conectar ao componente nativo.");
  }
}

async function request(
  method: string,
  path: string,
  body?: unknown,
): Promise<Response> {
  let res: Response;
  try {
    res = await fetch(`http://127.0.0.1:${getPort()}${path}`, {
      method,
      headers:
        body === undefined ? undefined : { "Content-Type": "application/json" },
      body: body === undefined ? undefined : JSON.stringify(body),
    });
  } catch {
    throw new OfflineError();
  }

  if (!res.ok) {
    const err = await res.json().catch(() => null);
    throw new ApiError(res.status, err?.err ?? `Erro ${res.status}.`);
  }
  return res;
}

/** true se o componente nativo responde. */
export async function ping(): Promise<boolean> {
  try {
    await request("GET", "/api/ping");
    return true;
  } catch {
    return false;
  }
}

/** Tabelas: `src` e "PASTA/NOME" (o componente grava <dados>/PASTA/NOME.csv). */
export const dados = {
  async list(src: string): Promise<Row[]> {
    const res = await request(
      "GET",
      `/api/dados?src=${encodeURIComponent(src)}`,
    );
    return res.json();
  },
  async create(src: string, row: Row): Promise<void> {
    await request("POST", "/api/dados", { src, new: row });
  },
  async update(src: string, old: Row, row: Row): Promise<void> {
    await request("PUT", "/api/dados", { src, old, new: row });
  },
  async remove(src: string, old: Row): Promise<void> {
    await request("DELETE", "/api/dados", { src, old });
  },
  async removeAll(src: string): Promise<void> {
    await request("DELETE", "/api/dados", { src, "delete-all": true });
  },
};

export const configuracao = {
  async get(): Promise<Config> {
    return (await request("GET", "/api/configuracao")).json();
  },
  async set(config: Config): Promise<void> {
    await request("POST", "/api/configuracao", config);
  },
};
