import { For, Show } from "solid-js";
import type { Component } from "solid-js";

import type { JSX } from "@solidjs/web";

import BadgeIcon from "~/assets/badge.svg";
import BusIcon from "~/assets/bus.svg";
import SchoolIcon from "~/assets/school.svg";

type Funcionalidade = {
  nome: string;
  icone: Component<JSX.SvgSVGAttributes<SVGSVGElement>>;
  /** Sem href = tela ainda não criada (o ícone aparece desativado). */
  href?: string;
};

type Categoria = {
  nome: string;
  itens: Funcionalidade[];
};

const categorias: Categoria[] = [
  {
    nome: "Transporte Escolar",
    itens: [
      { nome: "Formulário Transporte Regular", icone: BusIcon },
      { nome: "Formulário Escola Total", icone: SchoolIcon },
      { nome: "Crachás", icone: BadgeIcon, href: "/crachas" },
    ],
  },
];

const tileClass =
  "flex w-44 flex-col items-center gap-3 rounded-xl border border-slate-300/20 bg-slate-800 p-5 text-center";

function Tile(props: { item: Funcionalidade }) {
  const content = () => (
    <>
      <props.item.icone class="size-16" />
      <span class="font-semibold leading-tight">{props.item.nome}</span>
    </>
  );

  return (
    <Show
      when={props.item.href}
      fallback={
        <div class={`${tileClass} cursor-not-allowed opacity-40`}>
          {content()}
        </div>
      }
    >
      {(href) => (
        <a
          href={href()}
          class={`${tileClass} transition duration-150 ease-out hover:bg-slate-600 active:scale-[0.97]`}
        >
          {content()}
        </a>
      )}
    </Show>
  );
}

export function HomeScreen() {
  return (
    <div class="h-full overflow-y-auto p-8">
      <div class="mx-auto flex max-w-4xl flex-col gap-8">
        <For each={categorias}>
          {(categoria) => (
            <section class="flex flex-col gap-4">
              <h2 class="border-b border-slate-300/30 pb-2 text-xl font-bold">
                {categoria.nome}
              </h2>
              <div class="flex flex-wrap gap-4">
                <For each={categoria.itens}>
                  {(item) => <Tile item={item} />}
                </For>
              </div>
            </section>
          )}
        </For>
      </div>
    </div>
  );
}
