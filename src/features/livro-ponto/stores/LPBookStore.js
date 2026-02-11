import { useShallow } from "zustand/shallow";
import { createUserDataStore } from "../../../stores/userDataStore";

export const useLPBookStore = createUserDataStore("lp-book-info")

export const useLPBookList   = () => useLPBookStore(state => state.list);
export const useLPBookActions = () => useLPBookStore( useShallow(
  state => {
    console.log(state);
    return {
    updateList: state.updateList,
    addBook: state.addItem,
    delBook: state.removeItem,
  }}
));