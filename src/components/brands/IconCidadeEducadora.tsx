import { twMerge } from "tailwind-merge";

import cidadeEducadora from "~/assets/cidade-educadora.png";

type IconCidadeEducadoraProp = {
  class?: string;
};

export function IconCidadeEducadora(props: IconCidadeEducadoraProp) {
  return (
    <div
      class={twMerge(
        "flex flex-row items-center text-center gap-[0.25em] font-bold",
        props.class,
      )}
    >
      <img src={cidadeEducadora} class="h-[2.5em] w-auto"></img>
      <div class="flex flex-col">
        <span class="text-[1.125em] leading-[1.11]">Santos</span>
        <span class="text-[0.68em] leading-none">
          CIDADE <br /> EDUCADORA
        </span>
      </div>
    </div>
  );
}
