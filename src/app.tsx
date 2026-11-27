import { Loading } from "solid-js";

import { createRouter } from "@solidjs/router";
import { fileRoutes } from "@solidjs/router/fs";

import { pageRoutes } from "virtual:file-routes";

import "./app.css";

export const appStyle = {
  bgCol1: "bg-slate-800",
  bgCol2: "bg-slate-700",
  textCol1: "text-slate-100",
  textCol2: "",
  borderCol1: "border-slate-300",
};

const Router = createRouter({ routes: fileRoutes(pageRoutes) });

export default function App() {
  return <Router>{(props) => <Loading>{props.children}</Loading>}</Router>;
}
