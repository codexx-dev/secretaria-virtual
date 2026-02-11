import { useShallow } from "zustand/shallow";
import { createUserDataStore } from "../../../stores/userDataStore";

export const useCrachaStore = createUserDataStore("cracha-info");

export const useCrachaList   = () => useCrachaStore(state => state.list);
export const useCrachaActions = () => useCrachaStore( useShallow(
  state => ({
    updateList: state.updateList,
    addCracha: state.addItem,
    delCracha: state.removeItem,
    getCracha: state.getItem,
    updateCracha: state.updateItem,
  })
));

export function makeEmptCrachaInfo(){
  return {
    ume: 'UME Monte Cabrão', name: '', tel: '', photo: null, parentList: '', mainParent: ''
  }
}