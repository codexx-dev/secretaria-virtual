// Os .svg de src/assets sao importados como componentes (ver vite/svg.ts).
// O caminho mais especifico vence o "*.svg" (string) do vite/client.
declare module "~/assets/*.svg" {
  const component: import("solid-js").Component<
    import("@solidjs/web").JSX.SvgSVGAttributes<SVGSVGElement>
  >;
  export default component;
}
