import { twMerge } from "tailwind-merge";

import santosBrasao from "~/assets/santos-brasao.png";

type IconSantosProp = {
  class?: string;
};

export function IconSantos(props: IconSantosProp) {
  return (
    <div
      class={twMerge(
        "flex flex-col justify-center items-center font-bold",
        props.class,
      )}
    >
      <img src={santosBrasao} alt="" class="h-[2.8em] w-auto" />
      <span class="text-[0.5em] tracking-[0.25em] text-lime-400">
        PREFEITURA DE
      </span>
      <span class="mt-[0.33em] text-[2.25em] leading-0 text-emerald-600">
        Santos
      </span>
    </div>
  );
}

export default IconSantos;
