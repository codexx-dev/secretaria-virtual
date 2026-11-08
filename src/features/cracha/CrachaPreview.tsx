import { Index } from "solid-js";

import { IconCidadeEducadora } from "~/components/brands/IconCidadeEducadora";
import IconSantos from "~/components/brands/IconSantos";

export function CrachaPreview() {
  return (
    <div class="w-fit bg-white text-black">
      <div class="w-58 h-90 flex flex-col justify-between px-2 border-2 overflow-hidden font-bold [&_h1]:text-[14pt] [&_h2]:text-[9pt] text-center">
        <h1>UME Monte Cabrão</h1>

        <span class="font-normal text-[12pt]">(13)9999999</span>

        <div class="relative flex flex-row items-center text-[14pt]">
          <span class="absolute w-[1ch] ml-2 whitespace-break-spaces font-bold text-red-500">
            2 0 2 6
          </span>
          <div class="w-26 h-32 mx-auto border"></div>
        </div>

        <div class="h-26 border whitespace-break-spaces"></div>

        <h2>O uso deste crachá é obrigatório para utilizar o transporte.</h2>
      </div>

      <div class="w-58 h-90 px-1 border-2 overflow-hidden rotate-180">
        <div class="py-2 flex justify-between">
          <IconSantos class="text-[6.5px]" />

          <div class="flex flex-col justify-center text-center">
            <span class="font-bold text-[9.25px] leading-tight">
              PREFEITURA DE SANTOS
            </span>
            <span class="font-bold text-[6px] leading-tight">
              Secretaria de Educação
            </span>
          </div>

          <IconCidadeEducadora class="text-[8px]" />
        </div>

        <div class="px-1 font-bold [&_h2]:text-[9pt]">
          <h2 class="text-center">
            {`Ano: ${2026} - Turma: `} <span class="border-b">1-A</span>{" "}
          </h2>
          <h2 class="text-center">Autorizados a buscar no ponto:</h2>

          <ol class="list-decimal list-inside text-left *:my-2 text-[9pt]">
            <Index each={Array.from({ length: 7 })}>
              {(_, i) => (
                <li>
                  hello
                  <hr class="ml-3"></hr>
                </li>
              )}
            </Index>
          </ol>

          <h2>Telefones:</h2>
          <h2>11111</h2>
          <hr />
          <h2>Responsável:</h2>
          <h2>11111</h2>
          <hr />
        </div>
      </div>
    </div>
  );
}
